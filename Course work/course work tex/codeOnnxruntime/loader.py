import binascii

def convert_to_c_array(bytes) -> str:
  hexstr = binascii.hexlify(bytes).decode("UTF-8")
  hexstr = hexstr.upper()
  array = ["0x" + hexstr[i:i + 2] for i in range(0, len(hexstr), 2)]
  array = [array[i:i+10] for i in range(0, len(array), 10)]
  return ",\n  ".join([", ".join(e) for e in array])

onnx_binary = open("modelName.onnx", 'rb').read()
ascii_bytes = convert_to_c_array(onnx_binary)
c_file = "const unsigned char modelName_model_onnx[] = {\n  " 
    + ascii_bytes 
    + "\n};\nunsigned int modelName_model_onnx_len = " 
    + str(len(tflite_binary)) + ";
open("modelName.h", "w").write(c_file)