# LocalContrast
Simple Local Contrast Enhancement application.
Features 4 methods:
1. Global Histogram Equalization
2. Adaptive Histogram Equalization (AHE)
3. Contrast Limited Adaptive Histogram Equalization (CLAHE)
4. Unsharp mask
Works with color images using Lab (because contrast enhancement based on simple intensity calculated as 
RGB weighted sum tends to produce artifacts).

Application uses qt.conf to deploy on machines without Qt installed.

Tested with Qt 5.1 and OpenCV 2.4.9 on Windows 7. Had some issues running on Windows 8. 

To build application one should edit opencv.pri according to target OpenCV version and path.

Workflow is demonstrated in video available from https://www.dropbox.com/s/i40tn0s0ub8tpvj/contrast.mp4?dl=0

Possible improvements:
1. Use Gradient Domain-based approach with Poisson equation (as in single image Pseudo-HDR)
