# Seam-Carving
A MATLAB & C++ implementation of "Seam carving for content-aware image resizing" (Shamir & Avidan, 2007), and the forward energy function proposed in "Improved seam carving for video retargeting" (Rubinstein, Shamir & Avidan, 2008).

# General Overview & Main Goals
The diversity and versatility of display devices today imposes new demands on digital media. Sometimes, the size, or aspect ratio of an image must change. Standard techniques are oblivious to the image content.
More effective resizing can only be achieved by considering the image content and not only geometric constraints.

After understanding the problem we are facing, and proposing a general solution, what “pre – process” procedure should be applied before “fixing” any image?

# Saliency Maps
“In computer vision, a saliency map is an image that shows each pixel's unique quality.”
For our purposes – we are looking for a “map” which would hold each pixel’s “importance”.

Claim: “edges detection” methods can lead to a good “energy map”.

Shamir & Avidan, 2007 (L1 norm):
![image](https://user-images.githubusercontent.com/82894689/117580604-64d9a180-b101-11eb-8fd7-aa3e8956d310.png)


“Sobel filter”:

![image](https://user-images.githubusercontent.com/82894689/117580610-699e5580-b101-11eb-87ea-1f9b8a9d0b99.png)


![image](https://user-images.githubusercontent.com/82894689/117580645-a23e2f00-b101-11eb-81dd-4bbe721034a5.png)

Each pixel in the “energy map” holds the magnitude of the gradient calculated (using the Sobel filter or any other filter…)

# Previous Works

![image](https://user-images.githubusercontent.com/82894689/117580720-0103a880-b102-11eb-9f0b-7fde61bf489d.png)

Problem with previous works: 

All these methods achieve impressive results, but rely on traditional image resizing and cropping operations to actually change the size of the image.

We want to achieve more “flexibility” when resizing images.


