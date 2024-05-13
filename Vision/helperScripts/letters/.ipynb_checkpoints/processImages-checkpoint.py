import cv2
import numpy as np

for i in range(0, 283):
    img = cv2.imread(f'Vision/images/version1/Raw/U/img{i}.jpg')
    try:
        img.size
        grey = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
        ret, inverted = cv2.threshold(grey, 127, 255, cv2.THRESH_BINARY) 
        #img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        #mask = cv2.inRange(img_hsv, np.array([0,0,0]), np.array([255, 255, 255]))
        #normal_result = cv2.bitwise_and(img, img, mask=mask)
        #ret, inverted = cv2.threshold(normal_result, 0, 255, cv2.THRESH_BINARY) 
        cv2.imwrite(f'Vision/images/version1/Processed/U/img{i}.jpg', cv2.resize(inverted, (384, 218)))
    except:
        print(f"Image {i} does not exist!")