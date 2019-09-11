from skimage import measure
import matplotlib.pyplot as plt
import numpy as np
import cv2

def getmse(imageA, imageB):
	# the 'Mean Squared Error' between the two images is the
	# sum of the squared difference between the two images;
	# NOTE: the two images must have the same dimension
	err = np.sum((imageA.astype("float") - imageB.astype("float")) ** 2)
	err /= float(imageA.shape[0] * imageA.shape[1])
	
	# return the MSE, the lower the error, the more "similar"
	# the two images are
	return err

def getpsnr(imageA, imageB, mse):

	maxValue = (2**32)-1

	if (mse != 0):
		value = (maxValue**2)/(mse)
		res = 10 * np.log10(value)
	else:
		res = 0

	return res	

 


def compare_images(imageA, imageB, title, title2):
	# compute the mean squared error and structural similarity
	# index for the images
	m = getmse(imageA, imageB)
	p = getpsnr(imageA, imageB, m)
	s = measure.compare_ssim(imageA, imageB)
 
	# setup the figure
	fig = plt.figure(title)
	plt.suptitle("MSE: %.2f, PSNR:%.2f , SSIM: %.2f" % (m, p, s))
 
	# show first image
	ax = fig.add_subplot(1, 2, 1)
	plt.imshow(imageA, cmap = plt.cm.gray)
	plt.axis("off")
 
	# show the second image
	ax = fig.add_subplot(1, 2, 2)
	plt.imshow(imageB, cmap = plt.cm.gray)
	plt.axis("off")
 
	f = open("datos.csv","a+")
	f.write(title2 +"\n")
	f.write("MSE    PSNR      SSIM \n")
	f.write("%.2f  , %.2f , %.2f \n" % (m, p, s))

	# show the images
	plt.show()

# load the images -- the original, the original + contrast,
# and the original + photoshop
print("Insert the original image: ")
image1 = str(input())
print("Insert the stego image: ")
image2 = str(input())
print("Insert the name of the image: ")
title = str(input())

original = cv2.imread(image1)
stego1 = cv2.imread(image2)

 



# initialize the figure
fig = plt.figure("Images")
images = ("Original", original), ("Stego-Image-1", stego1)
 
# loop over the images
for (i, (name, image)) in enumerate(images):
	# show the image
	ax = fig.add_subplot(1, 3, i + 1)
	ax.set_title(name)
	plt.imshow(image, cmap = plt.cm.gray)
	plt.axis("off")
 
# show the figure
plt.show()
 
# compare the images

compare_images(original, stego1, "Original vs. Stego-Image1", title)
