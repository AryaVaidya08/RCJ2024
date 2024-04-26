import cv2 as cv
import numpy as np

vid = cv.VideoCapture(0)
low_red = np.array([0, 0, 0])
high_red = np.array([255, 255, 255])
low_green = np.array([50, 20, 85])
high_green = np.array([175, 255, 170])
low_yellow = np.array([25, 20, 85])
high_yellow = np.array([200, 200, 230])

while True:
    ret, frame = vid.read()
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    grey = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)

    red_mask = cv.inRange(hsv, low_red, high_red)
    red_result = cv.bitwise_and(frame,frame, mask=red_mask)

    yellow_mask = cv.inRange(hsv, low_yellow, high_yellow)
    yellow_result = cv.bitwise_and(frame, frame, mask=yellow_mask)

    green_mask = cv.inRange(hsv, low_green, high_green)
    green_result = cv.bitwise_and(frame,frame, mask=green_mask)

    grey = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    ret, binary_result = cv.threshold(grey, 127, 255, cv.THRESH_BINARY) 

    cv.imshow("Normal", cv.resize(frame, (384, 218)))
    cv.imshow("Red Mask", cv.resize(red_result, (384, 218)))
    cv.imshow("Yellow Mask", cv.resize(yellow_result, (384, 218)))
    cv.imshow("Green Mask", cv.resize(green_result, (384, 218)))
    cv.imshow("Letter Mask", cv.resize(binary_result, (384, 218)))

    cv.moveWindow("Red Mask", 0, 250)
    cv.moveWindow("Yellow Mask", 0, 500)
    cv.moveWindow("Green Mask", 400, 0)
    cv.moveWindow("Letter Mask", 400, 250)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv.destroyAllWindows()