# Seam-Carving
A MATLAB & C++ implementation of "Seam carving for content-aware image resizing" (Shamir & Avidan, 2007), and the forward energy function proposed in "Improved seam carving for video retargeting" (Rubinstein, Shamir & Avidan, 2008).

# General Overview & Main Goals
The diversity and versatility of display devices today imposes new demands on digital media. Sometimes, the size, or aspect ratio of an image must change. Standard techniques are oblivious to the image content.
More effective resizing can only be achieved by considering the image content and not only geometric constraints.

After understanding the problem we are facing, and proposing a general solution, what “pre – process” procedure should be applied before “fixing” any image?

# Saliency Maps
“In computer vision, a saliency map is an image that shows each pixel's unique quality.”
For our purposes – we are looking for a “map” which would hold each pixel’s “importance”.

Claim: “edges detection” methods can lead to a good “energy map”
Shamir & Avidan, 2007 (L1 norm):


“Sobel filter”:


