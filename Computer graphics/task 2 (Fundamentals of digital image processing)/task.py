import cv2
import numpy as np
import tkinter as tk
from tkinter import filedialog
from matplotlib import pyplot as plt 

class Application: 
    def __init__(self):
        self.app = tk.Tk()
        self.app.title("Image Processing Application")

        self.d = tk.IntVar()
        self.ksize = tk.IntVar()
        self.sigma_color = tk.DoubleVar()
        self.sigma_space = tk.DoubleVar()

        self.d.set(3)
        self.ksize.set(13)
        self.sigma_color.set(75)
        self.sigma_space.set(75)

        d_label = tk.Label(self.app, text="Parameter d:")
        d_label.pack()
        d_entry = tk.Entry(self.app, textvariable=self.d)
        d_entry.pack()

        ksize_label = tk.Label(self.app, text="Parameter ksize:")
        ksize_label.pack()
        ksize_entry = tk.Entry(self.app, textvariable=self.ksize)
        ksize_entry.pack()

        sigma_color_label = tk.Label(self.app, text="Parameter sigma_color:")
        sigma_color_label.pack()
        sigma_color_entry = tk.Entry(self.app, textvariable=self.sigma_color)
        sigma_color_entry.pack()

        sigma_space_label = tk.Label(self.app, text="Parameter sigma_space:")
        sigma_space_label.pack()
        sigma_space_entry = tk.Entry(self.app, textvariable=self.sigma_space)
        sigma_space_entry.pack()

        browse_button = tk.Button(self.app, text="Browse Image", command=self.browse_image)
        browse_button.pack()

        self.app.mainloop()

    def browse_image(self):
        file_path = filedialog.askopenfilename()
        if file_path:
            image = cv2.imread(file_path)

            d = self.d.get()
            ksize = self.ksize.get()
            sigma_color = self.sigma_color.get()
            sigma_space = self.sigma_space.get()
            print(d, " ", ksize, " ", sigma_color, " ", sigma_space)

            kernel = np.ones((5, 5), np.uint8)

            # Morphological
            dilated_image = cv2.dilate(image, kernel, iterations=1)
            eroded_image = cv2.erode(image, kernel, iterations=1)
            opened_image = cv2.morphologyEx(image, cv2.MORPH_OPEN, kernel)
            closed_image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)

            # Blurring images 
            blurred_image = cv2.blur(image, (ksize, ksize))

            median_image = cv2.medianBlur(image, ksize)

            kernel = np.zeros((ksize, ksize), np.float32)
            kernel[int((ksize - 1) / 2), :] = np.ones(ksize)
            kernel = kernel / ksize
            D_image = cv2.filter2D(image, -1, kernel)

            bilateral_image = cv2.bilateralFilter(image, d, sigma_color, sigma_space)

            # Processed images
            plt.figure(figsize=(38, 27))
            plt.subplot(331), plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB)), plt.title('Original Image')
            plt.subplot(332), plt.imshow(cv2.cvtColor(dilated_image, cv2.COLOR_BGR2RGB)), plt.title('Dilated Image')
            plt.subplot(333), plt.imshow(cv2.cvtColor(eroded_image, cv2.COLOR_BGR2RGB)), plt.title('Eroded Image')
            plt.subplot(334), plt.imshow(cv2.cvtColor(opened_image, cv2.COLOR_BGR2RGB)), plt.title('Opened Image')
            plt.subplot(335), plt.imshow(cv2.cvtColor(closed_image, cv2.COLOR_BGR2RGB)), plt.title('Closed Image')
            plt.subplot(336), plt.imshow(cv2.cvtColor(blurred_image, cv2.COLOR_BGR2RGB)), plt.title('Blurred Image')
            plt.subplot(337), plt.imshow(cv2.cvtColor(median_image, cv2.COLOR_BGR2RGB)), plt.title('Median Image')
            plt.subplot(338), plt.imshow(cv2.cvtColor(D_image, cv2.COLOR_BGR2RGB)), plt.title('2D Image')
            plt.subplot(339), plt.imshow(cv2.cvtColor(bilateral_image, cv2.COLOR_BGR2RGB)), plt.title('Bilateral Image')
            plt.show()

app = Application()