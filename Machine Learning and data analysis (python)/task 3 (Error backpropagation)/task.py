import numpy as np
import scipy as sp
import scipy.signal

class Module(object):
    """
    Basically, you can think of a module as of a something (black box)
    which can process `input` data and produce `ouput` data.
    This is like applying a function which is called `forward`:

        output = module.forward(input)

    The module should be able to perform a backward pass: to differentiate the `forward` function.
    Moreover, it should be able to differentiate it if is a part of chain (chain rule).
    The latter implies there is a gradient from previous step of a chain rule.

        input_grad = module.backward(input, output_grad)
    """

    def __init__(self):
        self._output = None
        self._input_grad = None
        self.training = True

    def forward(self, input):
        """
        Takes an input object, and computes the corresponding output of the module.
        """
        self._output = self._compute_output(input)
        return self._output

    def backward(self, input, output_grad):
        """
        Performs a backpropagation step through the module, with respect to the given input.

        This includes
         - computing a gradient w.r.t. `input` (is needed for further backprop),
         - computing a gradient w.r.t. parameters (to update parameters while optimizing).
        """
        self._input_grad = self._compute_input_grad(input, output_grad)
        self._update_parameters_grad(input, output_grad)
        return self._input_grad

    def _compute_output(self, input):
        """
        Computes the output using the current parameter set of the class and input.
        This function returns the result which will be stored in the `_output` field.

        Example: in case of identity operation:

        output = input
        return output
        """
        raise NotImplementedError

    def _compute_input_grad(self, input, output_grad):
        """
        Returns the gradient of the module with respect to its own input.
        The shape of the returned value is always the same as the shape of `input`.

        Example: in case of identity operation:
        input_grad = output_grad
        return input_grad
        """

        raise NotImplementedError

    def _update_parameters_grad(self, input, output_grad):
        """
        Computing the gradient of the module with respect to its own parameters.
        No need to override if module has no parameters (e.g. ReLU).
        """
        pass

    def zero_grad(self):
        """
        Zeroes `gradParams` variable if the module has params.
        """
        pass

    def get_parameters(self):
        """
        Returns a list with its parameters.
        If the module does not have parameters return empty list.
        """
        return []

    def get_parameters_grad(self):
        """
        Returns a list with gradients with respect to its parameters.
        If the module does not have parameters return empty list.
        """
        return []

    def train(self):
        """
        Sets training mode for the module.
        Training and testing behaviour differs for Dropout, BatchNorm.
        """
        self.training = True

    def evaluate(self):
        """
        Sets evaluation mode for the module.
        Training and testing behaviour differs for Dropout, BatchNorm.
        """
        self.training = False

    def __repr__(self):
        """
        Pretty printing. Should be overrided in every module if you want
        to have readable description.
        """
        return "Module"


class BatchNormalization(Module):
    EPS = 1e-3

    def __init__(self, alpha=0.):
        super(BatchNormalization, self).__init__()
        self.alpha = alpha
        self.moving_mean = 0.
        self.moving_variance = 1.

    def _compute_output(self, input):
        input = np.array(input)
        if self.training:
            mean = np.mean(input, axis=0)
            pre_res = input - mean
            var = np.sqrt(np.var(input, axis=0) + self.EPS)
            output = pre_res / var

            self.moving_mean = self.moving_mean * self.alpha + mean * (1 - self.alpha)
            self.moving_variance = self.moving_variance * self.alpha + np.sum(pre_res ** 2, axis=0) / input.shape[0] * (
                    1 - self.alpha)

            return output

        else:
            pre_res = input - self.moving_mean
            var = np.sqrt(self.moving_variance + self.EPS)
            output = pre_res / var

            return output

    def _compute_input_grad(self, input, output_grad):
        input = np.array(input)
        mean = np.mean(input, axis=0)
        pre_res = input - mean
        var = np.sqrt(np.var(input, axis=0) + self.EPS)

        dpre_res = output_grad / var
        dvar = np.sum(-output_grad * pre_res / var ** 2, axis=0)
        dpre_res = dpre_res + dvar * pre_res / var / input.shape[0]
        dmean = -np.sum(dpre_res, axis=0)
        input_grad = np.ones(input.shape) / input.shape[0] * dmean + dpre_res

        return input_grad

    def __repr__(self):
        return "BatchNormalization"


class ChannelwiseScaling(Module):
    """
       Implements linear transform of input y = \gamma * x + \beta
       where \gamma, \beta - learnable vectors of length x.shape[-1]
    """

    def __init__(self, n_out):
        super(ChannelwiseScaling, self).__init__()

        stdv = 1. / np.sqrt(n_out)
        self.gamma = np.random.uniform(-stdv, stdv, size=n_out)
        self.beta = np.random.uniform(-stdv, stdv, size=n_out)

        self.gradGamma = np.zeros_like(self.gamma)
        self.gradBeta = np.zeros_like(self.beta)

    def _compute_output(self, input):
        output = input * self.gamma + self.beta
        return output

    def _compute_input_grad(self, input, output_grad):
        grad_input = output_grad * self.gamma
        return grad_input

    def _update_parameters_grad(self, input, output_grad):
        self.gradBeta = np.sum(output_grad, axis=0)
        self.gradGamma = np.sum(output_grad * input, axis=0)

    def zero_grad(self):
        self.gradGamma.fill(0)
        self.gradBeta.fill(0)

    def get_parameters(self):
        return [self.gamma, self.beta]

    def get_parameters_grad(self):
        return [self.gradGamma, self.gradBeta]

    def __repr__(self):
        return "ChannelwiseScaling"


class Dropout(Module):
    def __init__(self, p=0.5):
        super(Dropout, self).__init__()

        self.p = p
        self.mask = []

    def _compute_output(self, input):
        # Your code goes here. ################################################
        input = np.array(input)
        if not self.training:
            output = input
            return output
        else:
            rnd = np.random.uniform(low=0., high=1., size=input.shape) > self.p
            output = input * rnd / (1 - self.p)
            self.mask = rnd
            return output

    def _compute_input_grad(self, input, output_grad):
        # Your code goes here. ################################################
        grad_input = output_grad * self.mask / (1 - self.p)
        return grad_input

    def __repr__(self):
        return "Dropout"


def mirror(item):
    item = np.array(item)
    item2 = []
    for it in item:
        it2 = []
        for i in it:
            it2.append(i[::-1])
        item2.append(it2[::-1])
    return np.array(item2)


class Conv2d(Module):
    def __init__(self, in_channels, out_channels, kernel_size):
        super(Conv2d, self).__init__()
        assert kernel_size % 2 == 1, kernel_size

        stdv = 1. / np.sqrt(in_channels)
        self.W = np.random.uniform(-stdv, stdv, size=(out_channels, in_channels, kernel_size, kernel_size))
        self.b = np.random.uniform(-stdv, stdv, size=(out_channels,))
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size

        self.gradW = np.zeros_like(self.W)
        self.gradb = np.zeros_like(self.b)

    def _compute_output(self, input):
        pad_size = self.kernel_size // 2
        # YOUR CODE ##############################
        # 1. zero-pad the input array
        # 2. compute convolution using scipy.signal.correlate(... , mode='valid')
        # 3. add bias value

        # self._output = ...

        input = np.array(input)
        input2 = []
        for item in input:
            item2 = []
            for it in item:
                it = np.pad(it, (pad_size,), 'constant', constant_values=0)
                item2.append(it)
            item2 = np.array(item2)
            input2.append(item2)
        input = np.array(input2)

        output = []
        for sample in input:
            out = []
            index = 0
            for item in self.W:
                out.append(scipy.signal.correlate(sample, item, mode='valid')[0] + self.b[index])
                index += 1
            out = np.array(out)
            output.append(out)
        self._output = np.array(output)
        return self._output

    def _compute_input_grad(self, input, gradOutput):
        pad_size = self.kernel_size // 2
        # YOUR CODE ##############################
        # 1. zero-pad the gradOutput
        # 2. compute 'self._input_grad' value using scipy.signal.correlate(... , mode='valid')

        gradOutput = np.array(gradOutput)
        input2 = []
        for item in gradOutput:
            item2 = []
            for it in item:
                it = np.pad(it, (pad_size,), 'constant', constant_values=0)
                item2.append(it)
            item2 = np.array(item2)
            input2.append(item2)
        gradOutput = np.array(input2)

        output = []
        for sample in gradOutput:
            out = []
            for item in self.W.transpose((1, 0, 2, 3)):
                out.append(scipy.signal.correlate(sample, mirror(item), mode='valid')[0])
            out = np.array(out)
            output.append(out)
        output = np.array(output)

        self._input_grad = output

        return self._input_grad

    def accGradParameters(self, input, gradOutput):
        pad_size = self.kernel_size // 2
        # YOUR CODE #############
        # 1. zero-pad the input
        # 2. compute 'self.gradW' using scipy.signal.correlate(... , mode='valid')
        # 3. compute 'self.gradb' - formulas like in Linear of ChannelwiseScaling layers

        input = np.array(input)
        input2 = []
        for item in input:
            item2 = []
            for it in item:
                it = np.pad(it, (pad_size,), 'constant', constant_values=0)
                item2.append(it)
            item2 = np.array(item2)
            input2.append(item2)
        input = np.array(input2)

        output = []
        index = 0
        for sample in input:
            i = []
            for it in gradOutput[index]:
                sm = []
                for sam in sample:
                    sm.append(scipy.signal.correlate(np.array([sam]), np.array([it]), mode='valid')[0])
                i.append(sm)
            output.append(i)
            index += 1
        out = np.array(output)

        self.gradW = np.sum(out, axis=0)

        b = []
        for item in gradOutput:
            b.append(np.sum(item, axis=(1, 2)))
        b = np.array(b)

        self.gradb = np.sum(b, axis=0)

    def zeroGradParameters(self):
        self.gradW.fill(0)
        self.gradb.fill(0)

    def getParameters(self):
        return [self.W, self.b]

    def getGradParameters(self):
        return [self.gradW, self.gradb]

    def __repr__(self):
        s = self.W.shape
        q = 'Conv2d %d -> %d' % (s[1], s[0])
        return q