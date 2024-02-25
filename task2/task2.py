import numpy as np
from PIL import Image
import sys


def find_angle(image):
    variances = []
    for angle in range(-90, 90):
        rot = image.rotate(angle, expand=True, fillcolor=255)
        variances.append(np.array(rot).sum(axis=1).var())
    
    rotate_back_angle = np.arange(-90, 90)[np.argmax(variances)]
    return -rotate_back_angle


def unrotate(path, save_path):
    """
    @param: path: str - path to the rotated image
    @param: save_path: str - path where to save the result
    @return: int - the angle of rotation in degrees
    """
    image = Image.open(path).convert("1")
    angle = find_angle(image)

    image = Image.open(path)
    restored = image.rotate(-angle, expand=True, fillcolor=(255, 255, 255))
    restored.save(save_path)

    return angle


if __name__ == "__main__":
    alpha = unrotate(sys.argv[1], sys.argv[2])
    print(alpha)
