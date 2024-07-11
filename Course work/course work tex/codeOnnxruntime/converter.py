import tf2onnx
import onnx

# Convert tflite model to ONNX
onnx_model, _ = 
        tf2onnx.convert
            .from_tflite(model_path="pathToTfliteModel.tflite", opset=13)

# Save the ONNX model
onnx.save(onnx_model, "pathToSaveOnnxModel.onnx")