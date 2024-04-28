import cv2
import numpy as np

low = np.array([0, 0, 0])
high = np.array([255, 255, 255])

kernel = np.array((
 [0, 1, 0],
 [1, -1, 1],
 [0, 1, 0]), dtype="int")

while True:

    constants = open("Vision/constants/letters.in", "r")
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

    print(low, high)

    img = cv2.imread('Vision/images/version1/Raw/H/img98.jpg')
    grey = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    #img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    #mask = cv2.inRange(img_hsv, low, high)
    #ret1, mask_inv = cv2.threshold(mask, 120, 255, cv2.THRESH_BINARY_INV)

    #normal_result = cv2.bitwise_and(img, img, mask=mask)


    ret, binary = cv2.threshold(grey, 127, 255, cv2.THRESH_BINARY)
    #grey = cv2.cvtColor(inverted, cv2.COLOR_BGR2GRAY)
    #ret, inverted2 = cv2.threshold(grey, 0, 255, cv2.THRESH_BINARY_INV) 
    # inverted_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # mask = cv2.inRange(inverted_hsv, low, high)
    # inverted_mask = cv2.bitwise_and(inverted, inverted, mask=mask)
    #output_image = cv2.morphologyEx(grey, cv2.MORPH_HITMISS, kernel)

    #cv2.imshow("thing", cv2.resize(mask_inv, (384, 218)))
    cv2.imshow("Normal", cv2.resize(img, (384, 218)))
    cv2.imshow('Binary', cv2.resize(binary, (384, 218))) 
    #cv2.imshow('Inverted', cv2.resize(inverted, (384, 218))) 
    #cv2.imshow('Inverted2', cv2.resize(inverted2, (384, 218))) 
    #cv2.imshow('Masked2', cv2.resize(output_image, (384, 218))) 


    #hit or miss
    #rate = 50

    #miss = cv2.resize(img, None , fx = rate, fy = rate, interpolation = cv2.INTER_NEAREST)
    #cv2.imshow("hiss", miss)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()