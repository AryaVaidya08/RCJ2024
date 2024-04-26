import cv2 as cv
import numpy as np
 
input_image = np.array((
 [0, 0, 0, 0, 0, 0, 0, 0],
 [0, 255, 255, 255, 0, 0, 0, 255],
 [0, 255, 255, 255, 0, 0, 0, 0],
 [0, 255, 255, 255, 0, 255, 0, 0],
 [0, 0, 255, 0, 0, 0, 0, 0],
 [0, 0, 255, 0, 0, 255, 255, 0],
 [0,255, 0, 255, 0, 0, 255, 0],
 [0, 255, 255, 255, 0, 0, 0, 0]), dtype="uint8")
 
kernel = np.array((
 [0, 1, 0],
 [1, -1, 1],
 [0, 1, 0]), dtype="int")
 
output_image = cv.morphologyEx(input_image, cv.MORPH_HITMISS, kernel)
 
rate = 50
kernel = (kernel + 1) * 127
kernel = np.uint8(kernel)
 
kernel = cv.resize(kernel, None, fx = rate, fy = rate, interpolation = cv.INTER_NEAREST)
 
input_image = cv.resize(input_image, None, fx = rate, fy = rate, interpolation = cv.INTER_NEAREST)
 
cv.imshow("thing", output_image)
output_image = cv.resize(output_image, None , fx = rate, fy = rate, interpolation = cv.INTER_NEAREST)
cv.imshow("Hit or Miss", output_image)
cv.moveWindow("Hit or Miss", 500, 200)
 
cv.waitKey(0)
cv.destroyAllWindows()