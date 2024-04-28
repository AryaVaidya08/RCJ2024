import os
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

import numpy as np
import cv2
import pandas as pd
from glob import glob
import tensorflow as tf
from keras import callbacks
from keras import optimizers
from model import build_model
from sklearn.model_selection import train_test_split

def createTrainAndTest():
    thing = pd.read_csv("./images/version1/Labeled/H/Version_4_26_24/BB_Images.csv")
    train, test = train_test_split(thing, test_size=0.2)
    train.to_csv("./images/version1/Labeled/H/Version_4_26_24/BB_Train_Images.csv")
    test.to_csv("./images/version1/Labeled/H/Version_4_26_24/BB_Test_Images.csv")

# Global Parameters
global height
global width
global num_classes

def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)

def load_labels(path):
    return ["H", "S", "U"]

def read_image_bbox(path, bbox, label_index):
    path = path.decode()
    image = cv2.imread(path)
    h, w, _ = image.shape
    image = cv2.resize(image, (width, height))
    image = (image - 127.5) / 127.5
    image = image.astype(np.float32)

    x1, y1, x2, y2 = bbox
    norm_x1 = float(x1/w)
    norm_y1 = float(y1/h)
    norm_x2 = float(x2/w)
    norm_y2 = float(y2/h)
    norm_bbox = np.array([norm_x1, norm_y1, norm_x2, norm_y2], dtype=np.float32)

    label = [0] * num_classes
    label[label_index] = 1
    class_label = np.array(label, dtype=np.float32)

    return image, norm_bbox, class_label

def parse(image, bbox, label):
    image, bbox, label = tf.numpy_function(read_image_bbox, [image, bbox, label], [tf.float32, tf.float32, tf.float32])
    image.set_shape((height, width, 3))
    bbox.set_shape((4))
    label.set_shape((num_classes))

    return (image), (bbox, label)

def tf_dataset(images, bboxes, labels, batch=8):
    ds = tf.data.Dataset.from_tensor_slices((images, bboxes, labels))
    ds = ds.map(parse).batch(batch).prefetch(10)
    return ds


def load_data(path, classes, train=True):
    images = []
    bboxes = []
    labels = []

    if train == True:
        df = pd.read_csv(os.path.join(path, "BB_Train_Images.csv"), header=None)
    else:
        df = pd.read_csv(os.path.join(path, "BB_Test_Images.csv"), header=None)
    
    for index, row in df.iterrows():
        if index == 0:
            continue
        name = row[2]
        width = int(row[3])
        height = int(row[4])
        x1 = int(row[5])
        y1 = int(row[6])
        x2 = int(row[7])
        y2 = int(row[8])
        label = int(row[9])
        label_name = classes[label]

        image = os.path.join(path, "images", name)
        bbox = [x1, y1, x2, y2]

        images.append(image)
        bboxes.append(bbox)
        labels.append(label)
    return images, bboxes, labels
        

def load_dataset(path, classes, split=0.2):
    ## train, validation, testing
    train_images, train_bboxes, train_labels = load_data(path, classes, train=True)

    split_size = int(len(train_images) * split)
    train_images, valid_images = train_test_split(train_images, test_size=split_size, random_state=42)
    train_bboxes, valid_bboxes = train_test_split(train_bboxes, test_size=split_size, random_state=42)
    train_labels, valid_labels = train_test_split(train_labels, test_size=split_size, random_state=42)

    test_images, test_bboxes, test_labels = load_data(path, classes, train=False)

    return (train_images, train_bboxes, train_labels), (valid_images, valid_bboxes, valid_labels), (test_images, test_bboxes, test_labels)

def main():
    np.random.seed(42)
    tf.random.set_seed(42)

    create_dir("files")

    height = 256
    width = 320
    batch_size = 16
    lr = 1e-4
    num_epochs = 500

    model_path = "files/model.h5"
    csv_path = "files/log.csv"
    dataset_path = "./images/version1/Labeled/H/Version_4_26_24/"

    classes = load_labels(dataset_path)
    num_classes = len(classes)

    (train_images, train_bboxes, train_labels), (valid_images, valid_bboxes, valid_labels), (test_images, test_bboxes, test_labels) = load_dataset(dataset_path, classes, split=0.2)
    print(f"{len(train_images)} - {len(train_bboxes)} - {len(train_labels)}")
    print(f"{len(valid_images)} - {len(valid_bboxes)} - {len(valid_labels)}")
    print(f"{len(test_images)} - {len(test_bboxes)} - {len(test_labels)}")

    train_ds = tf_dataset(train_images, train_bboxes, train_labels, batch=batch_size)
    valid_ds = tf_dataset(valid_images, valid_bboxes, valid_labels, batch=batch_size)

    model = build_model((height, width, 3))
    model.load_weights(model_path)
    model.compile(
        # loss="binary_crossentropy",
        loss = {
            "bbox": "binary_crossentropy",
            "label": "categorical_crossentropy"
        },
        optimizer=optimizers.optimizers.Adam(lr)
    )

    callbacks1 = [
        callbacks.callbacks.ModelCheckpoint(model_path, verbose=1, save_best_only=True),
        callbacks.callbacks.ReduceLROnPlateau(monitor='val_loss', factor=0.1, patience=5, min_lr=1e-7, verbose=1),
        callbacks.callbacks.CSVLogger(csv_path, append=True),
        callbacks.callbacks.EarlyStopping(monitor='val_loss', patience=20, restore_best_weights=False),
    ]

    model.fit(
        train_ds,
        epochs=num_epochs,
        validation_data=valid_ds,
        callbacks=callbacks1
    )
