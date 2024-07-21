import cv2
import numpy as np
import onnxruntime as ort
import time
import threading

# Initialize video capture
cap = cv2.VideoCapture(0)
cap.set(3, 1920)  # Use a smaller resolution
cap.set(4, 1080)

# Load ONNX model
sess = ort.InferenceSession('rvm_mobilenetv3_fp16.onnx')

# Initialize recurrent states
rec = [np.zeros([1, 1, 1, 1], dtype=np.float16) for _ in range(4)]
downsample_ratio = np.array([0.25], dtype=np.float32)

# Variables for FPS calculation
fps = 0
frame_count = 0
start_time = time.time()

# Function to process frames
def process_frame(frame):
    global rec
    # Preprocess frame
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert to RGB
    frame = cv2.resize(frame, (1280, 720))  # Resize to smaller resolution
    frame = frame.astype(np.float32) / 255.0  # Normalize to [0, 1]
    frame = frame.astype(np.float16)  # Convert to float16
    frame = np.transpose(frame, (2, 0, 1))  # Change shape to [C, H, W]
    frame = np.expand_dims(frame, axis=0)  # Add batch dimension [B, C, H, W]

    # Run inference
    fgr, pha, *rec = sess.run(None, {
        'src': frame, 
        'r1i': rec[0], 
        'r2i': rec[1], 
        'r3i': rec[2], 
        'r4i': rec[3], 
        'downsample_ratio': downsample_ratio
    })

    # Post-process the foreground and alpha
    fgr = fgr.squeeze().transpose(1, 2, 0)  # Remove batch dimension and change shape to [H, W, C]
    pha = pha.squeeze()  # Remove batch dimension

    # Convert the foreground and alpha to displayable format
    fgr = (fgr * 255).astype(np.uint8)
    pha = (pha * 255).astype(np.uint8)

    # Convert back to BGR for display
    fgr = cv2.cvtColor(fgr, cv2.COLOR_RGB2BGR)

    # Combine the foreground and alpha for visualization
    combined = cv2.merge((fgr, pha, pha))
    
    return pha

# Function to update FPS
def update_fps():
    global fps, frame_count, start_time
    frame_count += 1
    if frame_count >= 10:
        end_time = time.time()
        fps = frame_count / (end_time - start_time)
        start_time = end_time
        frame_count = 0

while True:
    ret, frame = cap.read()  # Capture frame-by-frame
    if not ret:
        break  # Break the loop if there's an issue capturing the frame

    # Process the frame in a separate thread
    combined = process_frame(frame)

    # Update FPS
    update_fps()

    # Display FPS on the frame
    cv2.putText(combined, f"FPS: {fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

    # Display the result
    cv2.imshow('Foreground with Alpha', combined)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break  # Exit loop on 'q' key press

cap.release()
cv2.destroyAllWindows()
