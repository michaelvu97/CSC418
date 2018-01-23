# CSC418 Computer Graphics Assignment 1
Michael Vu
1002473272
## 1. Curves

### 1.1 Convert Parametric to Implicit

$$
\begin{align}
x(t)^2&=4\sin^2(t)\\
y(t)^2&=25\sin^2(t)\cos^2(t)\\
y(t)^2&=\frac{25}{16}x^2(4-x^2)\\
0&=y^2+\frac{25}{16}x^4-\frac{25}{4}x^2
\end{align}
$$

### 1.2 Tangent Vector

$$
\begin{align}
\vec{T}(t)&=\frac{\langle\frac{dx}{dt},\frac{dy}{dt}\rangle}{||\langle\frac{dx}{dt},\frac{dy}{dt}\rangle||}\\
\frac{dx}{dt}&=2\cos(t)\\
\frac{dy}{dt}&=10\cos(2t)\\
\langle\frac{dx}{dt},\frac{dy}{dt}\rangle&=2\langle\cos (t),5\cos(2t)\rangle\\
\vec{T}(t)&=\frac{\langle \cos(t), 5\cos (2t)\rangle}{\sqrt{\cos^2(t)+25\cos^2(2t)}}
\end{align}
$$

### 1.3 Normal Vector

$$
\begin{align}
\vec{N}(t)&=\frac{\frac{d\vec{T}}{dt}}{||\frac{d\vec{T}}{dt}||}\\
\vec{N}(t) &= \frac{\langle -\sin (t) , -10\sin(2t) \rangle}{||\frac{d\vec{T}}{dt}||}
\end{align}
$$

**TODO fix this**

### 1.4 Symmetry

#### 1.4.1 X-axis  Symmetry

If the function is symmetrical about the X-axis, then $f(x,y)=f(x,-y)$.
$$
\begin{align}
f(x,y)&=f(x,-y)\\
y^2+\frac{25}{16}x^4-\frac{25}{4}x^2&=(-y)^2 + \frac{25}{16}x^4-\frac{25}{4}x^2\\
y^2&=(-y)^2\\
\text{left side} &= \text{right side}
\end{align}
$$
Therefore the function is symmetric about the X-axis.

#### 1.4.2 Y-axis Symmetry

If the function is symmetrical about the Y-axis, then $f(x,y)=f(-x,y)$.
$$
\begin{align}
f(x,y)&=f(-x,y)\\
y^2+\frac{25}{16}x^4 - \frac{25}{4}x^2&=y^2+\frac{25}{16}(-x)^2-\frac{25}{4}(-x)^2\\
\frac{25}{16}x^4-\frac{25}{4}x^2&=\frac{25}{16}(-x)^4 - \frac{25}{4}(-x)^2\\
\text{left side}&=\text{right side}
\end{align}
$$
Therefore the function is symmetric about the Y-axis.

### 1.5 Area

Since the function is symmetric about the X and Y axis, it is sufficient to find the area of the function in one quadrant and multiply by 4.
$$
\begin{align}
f(x)&=\sqrt{\frac{25}{4}x^2-\frac{25}{16}x^4}=\frac{5}{4}x\sqrt{4-x^2}\\
\frac14A&=\frac54 \int_0^2 x \sqrt{4-x^2} dx\\
\text{let }j&=4-x^2\\
dx&=\frac{1}{-2x}dj\\
A &= \frac52\int_0^4\sqrt{j} dj\\
A &=\frac52 \cdot\frac{16}{3}=15
\end{align}
$$

Therefore the area under the bowtie is $15$.

### 1.6 Perimeter

**TODO**

## 2. Transformations

#### 2.a Translation and Translation

The general form of a translation in homogenous form is
$$
T =
\begin{bmatrix}
1 & 0 & t_x \\
0 & 1 & t_y \\
0 & 0 & 1
\end{bmatrix}
$$
Two sequential translations
$$
T_1 =
\begin{bmatrix}
1 & 0 & t_{x1} \\
0 & 1 & t_{y1} \\
0 & 0 & 1
\end{bmatrix}\\
T_2 =
\begin{bmatrix}
1 & 0 & t_{x2} \\
0 & 1 & t_{y2} \\
0 & 0 & 1
\end{bmatrix}\\
T_1T_2=
\begin{bmatrix}
1 & 0 & t_{x1} \\
0 & 1 & t_{y1} \\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
1 & 0 & t_{x2} \\
0 & 1 & t_{y2} \\
0 & 0 & 1
\end{bmatrix}
=
\begin{bmatrix}
1 & 0 & t_{x1}+t_{x2} \\
0 & 1 & t_{y1}+t_{y2} \\
0 & 0 & 1
\end{bmatrix}
$$
By inspection, changin the order of $T1$ and $T2$ does not affect the resulting transformation matrix, therefore **translation and translation is commutative**.

#### 2.b Translation and Rotation

The general form of a rotation in homogenous form is
$$
T=
\begin{bmatrix}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0\\
0 & 0 & 1
\end{bmatrix}
$$
It follows that a rotation followed by a translation has the form
$$
T_1=
\begin{bmatrix}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0\\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
1 & 0 & t_x \\
0 & 1 & t_y \\
0 & 0 & 1
\end{bmatrix}
=
\begin{bmatrix}
\cos\theta & -\sin\theta & t_x\cos\theta -t_y\sin\theta \\
\sin\theta & \cos\theta & t_x\sin\theta + t_y\cos\theta \\
0 & 0 & 1
\end{bmatrix}
$$
Conversely, a translation followed by a rotation has the form
$$
T_2=
\begin{bmatrix}
1 & 0 & t_x \\
0 & 1 & t_y \\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0\\
0 & 0 & 1
\end{bmatrix}
=
\begin{bmatrix}
\cos\theta & -\sin\theta & t_x \\
\sin\theta & \cos\theta & t_y \\ 
0 & 0 & 1
\end{bmatrix}
$$
$T_1 \neq T_2$, therefore **translation and rotation are not commutative**.

### 2.c Scaling and Rotation, with different fixed points

**TODO**

### 2.d Scaling and Scaling, with the same fixed point.

Two combined scaling transformations have the form
$$
T_1*T_2=
\begin{bmatrix}
S_{x1} & 0 & 0 \\
0 & S_{y1} & 0\\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
S_{x2} & 0 & 0 \\
0 & S_{y2} & 0\\
0 & 0 & 1
\end{bmatrix}=\begin{bmatrix}
S_{x1}S_{x2} & 0 & 0 \\
0 & S_{y1}S_{y2} & 0\\
0 & 0 & 1
\end{bmatrix}
$$
which is clearly commutative for *any* input points, therefore **scaling and scaling with the same fixed point is commutative**.

## 3. Homography

### 3.1 Affine Transform Derivation

$$
\begin{bmatrix}
(1,0)\\
(0,1)\\
(1,1)\\
(0,0)\\
\end{bmatrix}
\Rightarrow
\begin{bmatrix}
(6,2)\\
(7,3)\\
(6,3)\\
(7,2)
\end{bmatrix}
$$

A trace of the transformation reveals that it is a x-scaling by a factor of $-1$ about the origin, and a translation by $\langle 7,2 \rangle$. The combined transformation matrix is
$$
T=
\begin{bmatrix}
1 & 0 & 7 \\
0 & 1 & 2\\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
-1 & 0 & 0 \\
0 & 1 & 0\\
0 & 0 & 1
\end{bmatrix}
=
\begin{bmatrix}
-1 & 0 & 7 \\
0 & 1 & 2 \\
0 & 0 & 1
\end{bmatrix}
$$

### 3.2 Map (2,5) Under the Transformation

$$
\vec{p}=
\begin{bmatrix}
-1 & 0 & 7 \\
0 & 1 & 2 \\
0 & 0 & 1
\end{bmatrix}
\begin{bmatrix}
2 \\
5 \\
1
\end{bmatrix}
=
\begin{bmatrix}
5\\
7\\
1
\end{bmatrix}
\rightarrow(5,7)
$$

The point $(2,5)$ maps to $(5,7)$ under this transformation.

## 4. Polygons

**TODO**