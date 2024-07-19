import cv2
import cvzone

from cvzone.SelfiSegmentationModule import SelfiSegmentation

cap = cv2.VideoCapture(0)
cap.set(3, 1920)
cap.set(4, 1080)

segmentor = SelfiSegmentation()
fpsReader = cvzone.FPS()

while True:
    _, frame = cap.read()
    outImg = segmentor.removeBG(frame, (255, 0, 0), threshold=0.8)
    
    img = cvzone.stackImages([frame, outImg], 2, 1)
    _, img = fpsReader.update(img, color=(0, 0, 255))
    cv2.imshow("img", img)
    cv2.waitKey(1)