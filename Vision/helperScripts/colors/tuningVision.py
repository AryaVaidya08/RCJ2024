import cv2 as cv
import numpy as np

vid = cv.VideoCapture(0)

low = np.array([0, 0, 0])
high = np.array([255, 255, 255])

while True:

    constants = open("Vision/constants/values.in", "r")
    values = constants.readlines()
    values[0] = values[0][:-1]
    values = [[int(num) for num in value.split()] for value in values]
    if len(values) != 2:
        low = np.array([0, 0, 0])
        high = np.array([255, 255, 255])
        print(low, high)
    else:
        low = np.array(values[0])
        high = np.array(values[1])
        print(low, high)
    constants.close()

    ret, frame = vid.read()
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    mask = cv.inRange(hsv, low, high)
    result = cv.bitwise_and(frame,frame, mask=mask)

    cv.imshow("Normal", cv.resize(frame, (384, 218)))
    cv.imshow("Mask", cv.resize(result, (384, 218)))
    
    cv.moveWindow("Mask", 0, 250)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv.destroyAllWindows()