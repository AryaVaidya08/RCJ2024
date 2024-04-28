import cv2
import numpy as np
import os

for i in range(0, 283):
    img = cv2.imread(f'Vision/images/version1/Raw/U/img{i}.jpg')
    try:
        img.size
    except:
        print(f"Image {i} does not exist!")
        continue

    print("Showing Picture ", i)
    cv2.imshow("Thing", img)

    

    if cv2.waitKey(0) & 0xFF == ord('d'):
        print("Deleting...")
        os.remove(f"Vision/images/version1/Raw/U/img{i}.jpg")
    if cv2.waitKey(0) & 0xFF == ord('q'):
        print("Keeping...")
        cv2.destroyAllWindows()