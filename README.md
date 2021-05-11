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

# Object Removal / Object Preservation
soft constraints for object removal / object preservation can be applied by the user.
This can be implemented by adding “positive energy” to objects which the user would like to preserve, and “lower energy” to objects which the user would like to remove:

![image](https://user-images.githubusercontent.com/82894689/117581221-6c4e7a00-b104-11eb-86a4-0dcd723c335c.png)

![image](https://user-images.githubusercontent.com/82894689/117581228-7a9c9600-b104-11eb-8c71-5620e9e80a90.png)

<b>Find the missing shoe:</b>

![image](https://user-images.githubusercontent.com/82894689/117581250-8f792980-b104-11eb-9709-fa9b3acadf29.png)

# Multi-Size Images (Live Resizing)
we have assumed that the user knows the target size ahead of time, but this might not be possible in some cases.
<b>Example:</b> web pages. Designer can’t know ahead of time at what resolution the page will be displayed.
We would like to achieve “live” resizing.
To address this issue, we use a representation of multi-size images that encodes an entire range of retargeting. This information has a very low memory footprint, can be computed in a couple of seconds as a pre-processing step, and allows the user to retarget an image continuously in real time.

This process takes a bit more time at first (pre- processing…), but after the first stage, the resizing will run in “real time”!
Every pixel, except of its “energy”, will also hold the index of the seam that will (potentially) remove it.
The user then resizes the image, and only the relevant pixels will remain.

![image](https://user-images.githubusercontent.com/82894689/117581311-d23b0180-b104-11eb-8031-0a1fb2df4596.png)

# Modifications
A problem may arise – since our main concern is to find the “best seam” energy wise,  it’s possible that removing such a seam (so called) adds new energy to the image!

![image](https://user-images.githubusercontent.com/82894689/117581345-f5fe4780-b104-11eb-92bd-f67cb7eefb1e.png)

Our algorithm is not sensitive to such problems… can we fix this?

The forward energy modification was suggested by Rubinstein, Shamir and Avidan (2008).
It can be added “on top” of the basic energy calculation we reviewed earlier.

![image](https://user-images.githubusercontent.com/82894689/117581375-13cbac80-b105-11eb-94c7-ce670f8fae97.png)

![image](https://user-images.githubusercontent.com/82894689/117581384-1dedab00-b105-11eb-9d59-af17cc45f572.png)

# Later Works
Panozzo et al. (Robust Image Retargeting via Axis-Aligned Deformation, 2012).
The work focuses on continuous retargeting.
In contrast to “seam – carving”, the suggested approach doesn’t look for seams to “remove”, rather tries to warp the image as less as possible in the “important” parts, and allows more warping in “unimportant” parts.
Results usually look smoother, but sometimes carving might be preferable (object removal for example).

![image](https://user-images.githubusercontent.com/82894689/117581411-4bd2ef80-b105-11eb-92fb-81b0d63fc5a5.png)

![image](https://user-images.githubusercontent.com/82894689/117581423-53929400-b105-11eb-8f90-e0e2e62dcd97.png)

![image](https://user-images.githubusercontent.com/82894689/117581429-6016ec80-b105-11eb-825e-0a541ab4fa77.png)

# Later Works – Cont.: Deep Learning Methods
Zhao et al. – “Saliency Detection by Multi-Context Deep Learning”, CVPR 2015:
Low-level saliency cues or priors do not produce good enough saliency detection results, especially when the salient object presents in a low-contrast background with confusing visual appearance.
Deep Convolutional Neural Networks are employed to model saliency of objects in images. Global context and local context are both taken into account, and are jointly modeled in a unified multi-context deep learning framework.

![image](https://user-images.githubusercontent.com/82894689/117581460-82a90580-b105-11eb-8258-4bb981633999.png)

A dark gray house appears in dark yellow bush. Objects like the 
house cannot be classified as salient objects from the low-contrast 
background based on “low-level” saliency cues (which are used in the 
more basic approaches, such as the original “Seam Carving” 2007
paper) . This advanced  method  enables these kind of classifications. 

# Comparisons to Original Paper

![image](https://user-images.githubusercontent.com/82894689/117581484-a9673c00-b105-11eb-9af4-28ee292a45eb.png)

![image](https://user-images.githubusercontent.com/82894689/117581494-b1bf7700-b105-11eb-8893-3f4c037c7ec2.png)

![image](https://user-images.githubusercontent.com/82894689/117581499-b97f1b80-b105-11eb-9f27-f8756f589077.png)

# References
- Seam Carving for Content-Aware Image Resizing  (Shamir & Avidan, 2007)
- Improved Seam Carving for Video Retargeting (Rubinstein, Shamir and Avidan, 2008)
- Robust Image Retargeting via Axis-Aligned Deformation (Panozzo et al., 2012)
- Saliency Detection by Multi-Context Deep Learning (Zhao et al., 2015)
- Seam Carving | Week 2 | 18.S191 MIT Fall 2020 | Grant Sanderson - https://www.youtube.com/watch?v=rpB6zQNsbQU

# Added Files
this project contains several files:

- A recording of the lecture (the lecture was given at Bar-Ilan Univeristy, May 2021, as part of the "Digital Geometric Processing 2" course): https://drive.google.com/file/d/1q_Ono-onGnJyOpaV39UIg1lnL5LRMwcn/view?usp=sharing

- A PowerPoint presentation which was used in the lecture (Seam – Carving ppt.pptx)
- The MATLAB app .rar file (Seam Carving Application - 21.4.rar) - contains the .exe file and a README file explaining how to use the application.
- The MATLAB app code (app code.txt)
- C++ files used in this project 
- MEX files used in this project (originated from the C++ files)

# How to use this Application

![image](https://user-images.githubusercontent.com/82894689/117581908-0e239600-b108-11eb-842e-d8ab587212ba.png)

<b>INSTRUCTIONS:</b>

this tool contains 3 different formats:

1)seam view - this format enables the user to view seam removals in both dimensions.

2)live size reduce - 1D - this format enables the user to resize the image in one specific dimension in real time.

3)resizing (enlarging inc.) - this format enables the user to resize (reduce or enlarge) the image in both dimensions.

<b>FURTHER EXPLANATIONS:</b>

1)seam view - 
the user is asked to provide 2 non-positive integers for the seam removal process. this format also comes along with a 
"energy insertion prevention", which adds the "forward energy" introduced in the later "Improved Seam Carving for Video Retargeting" paper.

2)live size reduce - 1D -
the user is asked to provide 1 non-positive integer (while the other one remains 0). after clicking the "RUN" button, the user
can now change the negative value to any value between 0 and 50% the size of the image in that specific dimension, followed
by a click on the "update" button. a real time change will appear in the figure.

3)resizing (enlarging inc.) -
the user is asked to provide 2 integers.

<b>ADDITIONAL FEATURES:</b>

1)"save image" button - after running any format, clicking this button enables to user to save the updated image.

2)"restart" button

3)"region protection"/"region removal" - the user is given an option to draw regions in the image which he would like to
preserve/delete. to use this feature, turn "on" relevant switch, and pick the number of regions to be preserved/deleted. 















