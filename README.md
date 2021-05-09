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

# Saliency Maps – Cont.
Finding a good saliency map, and actually resizing an image, are two different problems – each can be solved separately.  
Different saliency maps can result in different image resizing – we can choose between many methods, according to what we would like to achieve.

Given a saliency map of some image “G” (the number in each pixel represents the pixel’s “importance”): 

![image](https://user-images.githubusercontent.com/82894689/117580791-563fba00-b102-11eb-8e2d-ef987b20c0c2.png)

Let’s say we want to reduce the size of the image “G” by one column. What’s the best way to achieve this?

# Image Resizing
Reasonable assumption: our goal is to preserve as much energy as possible.
Different proposals from the paper:

![image](https://user-images.githubusercontent.com/82894689/117580826-84bd9500-b102-11eb-9f81-67484f8ad124.png)

we want to get “the best of both worlds” – keeping the energy, while trying not do deform the image too much.
seam: “A seam is a connected path of low energy pixels crossing the image from top to bottom, or from left to right.” (Shamir & Avidan, 2007):

![image](https://user-images.githubusercontent.com/82894689/117580873-ae76bc00-b102-11eb-9869-9fd4dd09c0cd.png)

Back to our previous example – let’s say we want to start from the “0.3” pixel. A greedy approach can be taken:

![image](https://user-images.githubusercontent.com/82894689/117580897-cea67b00-b102-11eb-9a5d-f1a57c2644f6.png)

Our current goal is to find a seam s* , such that:

![image](https://user-images.githubusercontent.com/82894689/117580941-e847c280-b102-11eb-90be-6cd1afda257b.png)


A greedy approach might be fast, but not optimal.
<b>Solution:</b> use dynamic programming!

![image](https://user-images.githubusercontent.com/82894689/117580977-0f9e8f80-b103-11eb-8f74-4c28bc42a64f.png)

After each seam removal, the energy map changes.
This fact requires us to compute the dynamic map again after each seam removal.

Seam carving strikes the best balance between the demands for energy preservation and visual coherency:

![image](https://user-images.githubusercontent.com/82894689/117580998-3230a880-b103-11eb-9804-5bdcfeb03472.png)

# Results - Width Reduce

![image](https://user-images.githubusercontent.com/82894689/117581015-4674a580-b103-11eb-8d36-11ec28e5d22f.png)

# Results – Height Reduce 

![image](https://user-images.githubusercontent.com/82894689/117581038-6b691880-b103-11eb-8bfd-e3470abcbeb9.png)

# Image Resizing – Optimal Seam Order (both dimensions)

Sometimes, we want to resize an image in both dimensions (for now let’s assume we want to reduce the size in both dimensions).

![image](https://user-images.githubusercontent.com/82894689/117581071-9bb0b700-b103-11eb-9916-85d8fd92587b.png)

What is the correct order in which we need to remove the seams? 

Optimal solution:

![image](https://user-images.githubusercontent.com/82894689/117581085-b2570e00-b103-11eb-960a-45b9068f5038.png)

Using dynamic programming, we can achieve this optimal result – but it is costly! (running-time wise…)

# Image Enlarging
<b>Paper’s approach:</b> To enlarge an image we approximate an ‘inversion’ of this time evolution and insert new ‘artificial’ seams to the image. Hence, to enlarge the size of an image I by one we compute the optimal vertical (horizontal) seam s on I and duplicate the pixels of s by averaging them with their left and right neighbors (top and bottom in the horizontal case).
<b>Downside:</b> since the insertions are “artificial”, the final result might look “artificial” as well…

![image](https://user-images.githubusercontent.com/82894689/117581143-019d3e80-b104-11eb-9d4d-f726d90cd798.png)

Easier approaches can also be implemented (isotropic scaling, and then carving in one dimension, for example - this approach is the one implemented in my implementation of the algorithm).





