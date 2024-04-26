import cv2 as cv
import numpy as np

vid = cv.VideoCapture(0)
low_red = np.array([0, 0, 0])
high_red = np.array([255, 255, 255])

while True:
    ret, frame = vid.read()
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    red_mask = cv.inRange(hsv, low_red, high_red)
    red_result = cv.bitwise_and(frame,frame, mask=red_mask)

    cv.imshow("Normal", cv.resize(frame, (384, 218)))
    cv.imshow("Red Mask", cv.resize(red_result, (384, 218)))
    cv.moveWindow("Red Mask", 0, 250)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv.destroyAllWindows()