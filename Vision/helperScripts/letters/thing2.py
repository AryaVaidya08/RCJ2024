import cv2

img = cv2.imread("Vision/images/version1/Processed/H/img0.jpg")
print(img)

img2 = (img - 127.5) / 127.5
print(img2)
cv2.imshow("r", img)
cv2.imshow("g", img2)

cv2.waitKey(0)
cv2.destroyAllWindows()


