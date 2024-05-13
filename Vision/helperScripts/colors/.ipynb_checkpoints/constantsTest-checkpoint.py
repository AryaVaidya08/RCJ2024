import cv2 as cv
import numpy as np

low_red = np.array([0, 0, 0])
high_red = np.array([255, 255, 255])

constants = open("Vision/constants/values.in", "r")
values = constants.readlines()
values[0] = values[0][:-1]
values = [[int(num) for num in value.split()] for value in values]
if len(values) != 2:
    low_red = np.array([0, 0, 0])
    high_red = np.array([255, 255, 255])
    print(low_red, high_red)
else:
    low_red = np.array(values[0])
    high_red = np.array(values[1])
    print(low_red, high_red)
constants.close()