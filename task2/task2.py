import numpy as np
from PIL import Image
import cv2
import sys


def unrotate(path, save_path, delta=15, precision=1):
    """
    @param: path: str - path to the rotated image
    @param: save_path: str - path where to save the result
    @param: delta: int - parameter responsible for speed-quality trade-off
    smaller larger leads to higher speed, smaller delta improves quality
    delta should be in range [1, min(image_width, image_height) / 2))
    @param: precision: int - set the precision of the rotation angle (step of the grid search).
    Larger values increase speed and decrease quality.
    For example, for precision=5 candidates will be: -90, -85, -80, ..., 85, 90
    @return: int - the angle of rotation in degrees
    """
    image = Image.open(path)
    np_image_2d = np.mean(np.array(image), axis=2).astype(np.float32)
    blank = np.zeros_like(np_image_2d)

    height, width = np_image_2d.shape

    variances = []
    for angle in range(-90, 91, precision):
        conv1d_results = []
        radians = angle * np.pi / 180
        if -45 <= angle < 45:
            step = np.round(width / 2 * np.tan(radians)).astype(int)
            for y in range(-abs(step) // 2, height + abs(step) // 2 + 1, delta):
                cv2.line(blank, (width, y + step), (0, y - step), 255., 2)
                conv1d_results.append(np_image_2d[blank > 1e-9].mean())
                blank[:, :] = 0.
        else:
            step = np.round(height / 2 * np.cos(radians) / np.sin(radians)).astype(int)
            for x in range(-abs(step) // 2, width + abs(step) // 2 + 1, delta):
                cv2.line(blank, (x - step, 0), (x + step, height), 255., 2)
                conv1d_results.append(np_image_2d[blank > 0].mean())
                blank[:, :] = 0
        variances.append(np.var(conv1d_results))
    
    rotate_back_angle = np.arange(-90, 91, precision)[np.argmax(variances)]
    restored = image.rotate(rotate_back_angle, expand=True, fillcolor=(255, 255, 255))
    restored.save(save_path)

    return -rotate_back_angle


if __name__ == "__main__":
    delta = int(sys.argv[3]) if len(sys.argv) > 3 else 15
    precision = int(sys.argv[4]) if len(sys.argv) > 4 else 1
    alpha = unrotate(sys.argv[1], sys.argv[2], delta=delta, precision=precision)
    print(alpha)
