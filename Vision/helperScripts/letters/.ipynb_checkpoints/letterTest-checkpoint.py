import cv2 
import pytesseract
import numpy as np

for i in range(1, 8):
    img = cv2.imread(f'Vision/images/S_img_{i}.jpg')
    grey = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, inverted = cv2.threshold(img, 120, 255, cv2.THRESH_BINARY_INV) 
    inverted_hsv = cv2.cvtColor(inverted, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(inverted_hsv, np.array([0,0,0]), np.array([0, 0, 255]))
    inverted_mask = cv2.bitwise_and(inverted, inverted, mask=mask)
    cv2.imwrite(f'./invertedMask{i}.jpg', cv2.resize(inverted_mask, (384, 218)))

# img = cv2.imread('Vision/images/S_img_1.jpg')
# grey = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# ret, inverted = cv2.threshold(img, 120, 255, cv2.THRESH_BINARY_INV) 

# inverted_hsv = cv2.cvtColor(inverted, cv2.COLOR_BGR2HSV)
# mask = cv2.inRange(inverted_hsv, np.array([0,0,0]), np.array([0, 0, 255]))
# inverted_mask = cv2.bitwise_and(inverted, inverted, mask=mask)
# cv2.imshow('Inverted Mask', cv2.resize(inverted_mask, (384, 218))) 

# cv2.imwrite('./invertedMask.jpg', inverted_mask)


#cv2.moveWindow()
#cv2.moveWindow("img", 385 * ((i-1) // 3), 250 * ((i - 1) % 3))


# Adding custom options
#custom_config = r'--oem 3 --psm 6'
#print(pytesseract.image_to_string(img, config=custom_config))
#pytesseract.image_to_string(img, config=custom_config)

# cv2.waitKey(0)
# cv2.destroyAllWindows()