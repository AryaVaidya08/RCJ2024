import cv2 as cv
import numpy as np
import time

startTime = int(time.time())
print(int(startTime))

selectedLetter = "H"
count = 148
lastFrameTaken = 0

vid = cv.VideoCapture(0)

while True:
    ret, frame = vid.read()

    cv.imshow("Normal", cv.resize(frame, (384, 218)))

    time1 = int(time.time()) - startTime

    if time1 % 3 == 0 and time1 != lastFrameTaken:
        lastFrameTaken = time1
        print(f"Taking picture!{count}")
        cv.imwrite(f"Vision/images/version1/{selectedLetter}/img{count}.jpg", frame)
        count += 1
    

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()
cv.destroyAllWindows()