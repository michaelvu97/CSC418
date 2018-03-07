

# CSC418 Assignment 2

**Michael Vu**

**<CDF ID>**

## 1. Transformations

### 1.1 2D Affine Transformation

$$
\begin{align}
TA&=B\\
\begin{bmatrix}
t_{11} & t_{21} & t_{31}\\
t_{12} & t_{22} & t_{32}\\
t_{13} & t_{23} & t_{33}
\end{bmatrix}
\begin{bmatrix}
a_{1x} & a_{2x} & a_{3x}\\
a_{1y} & a_{2y} & a_{3y}\\
1 & 1 & 1
\end{bmatrix}
&=
\begin{bmatrix}
b_{1x} & b_{2x} & b_{3x}\\
b_{1y} & b_{2y} & b_{3y}\\
c_1 & c_2 & c_3
\end{bmatrix}
\\
TAA^{-1}=T&=BA^{-1}\\
\end{align}
$$


$$
\det(A)=(a_{2x}a_{3y}-a_{2y}a_{3x})+(a_{3x}a_{1y}-a_{1x}a_{3y})+(a_{1x}a_{2y}-a_{1y}a_{2x})
$$
The mapping is fully determined when $\det(A)\neq 0$ , since the matrix would not be invertible and the transformation could not be specified.
$$
\begin{align}
\det(A)=0&=(a_{2x}a_{3y}-a_{2y}a_{3x})+(a_{3x}a_{1y}-a_{1x}a_{3y})+(a_{1x}a_{2y}-a_{1y}a_{2x})\\
0&=a_2\times a_3+a_3 \times a_1 + a_1 \times a_2\\
0&=\sin\theta_{23}+\sin\theta_{31}+\sin\theta_{12}
\end{align}
$$
Using CFG, this represents the sum of lines formed from $p_1\rightarrow p_2$, $p_2 \rightarrow p_3$, and $p_3 \rightarrow p_1$.

### 1.2 ???

### 1.3 ???

## 2. Viewing and Projection

### 2.1 Pinhole Camera

Since light rays propagate by straight lines, a screen pixel can be determined as a sum of the light sources in a cone through the aperture (assuming a circular aperture). As the aperture size decreases to a "pinhole", the cone of light rays collapses into approximately a straight line through the aperture. Assuming the aperture is $(0,0)$, screen pixel $(x,y, depth)$  will be determined by the light source in the direction $\langle-x,-y,-depth \rangle$. The resulting image formed is an $xy$ inverted image of the world.

### 2.2 World to Camera Transformation

#### Translation

The first and most simple transformation that can be applied is a translation from the world's centre to the camera's centre.
$$
T_1= \textbf{0 } \rightarrow\textbf{c }=
\begin{bmatrix}
1 & 0 & 0 & -c_x\\
0 & 1 & 0 & -c_y\\
0 & 0 & 1 & -c_z\\
0 & 0 & 0 & 1
\end{bmatrix}
$$
Assuming that $\textbf{p}$ is given in world coordinates, let $\textbf{p}'=T_1\textbf{p}$ in translated camera coordinates. Find the ray from $\textbf{p}'$ to the origin:
$$
\vec{r}=\textbf{p}'\rightarrow\textbf{0}=\langle -p'_x,-p'_y,-p'_z, 1 \rangle=\langle  c_x-p_y,c_y-p_y,c_z-p_z,1\rangle\\
\hat{r}=\frac{\vec{r}}{||\vec{r}||} \text{ for convenience}
$$
First, rotate about $\hat{x}$ to make $\hat{y}'$  perpendicular to $\vec{r}$.
$$
\begin{align}
0&=(T_{rotx}\hat{y})\cdot\hat{r}\\
0&=\begin{bmatrix}
1 & 0 & 0\\
0 & \cos\theta & -\sin\theta\\
0 & \sin\theta & \cos\theta
\end{bmatrix}
\begin{bmatrix}
0\\1\\0
\end{bmatrix}
\cdot\hat{r}\\
0&=\begin{bmatrix}
0\\
\cos\theta\\
\sin\theta
\end{bmatrix}
\cdot
\hat{r}
\\
0&=\hat{r}_y\cos\theta+\hat{r}_z\sin\theta\\
\implies\theta&=\arctan(-\frac{\hat{r}_y}{\hat{r}_z})\\
\theta&=-\arctan\Big(\frac{c_y-p_y}{c_z-p_z}\Big)\\
\implies T_{rotx}&=\begin{bmatrix}
1 & 0 & 0\\
0 & \cos\Bigg(\frac{c_z-p_z}{\sqrt{(c_z-p_z)^2+(c_y-p_y)^2}}\Bigg) & -\sin\Bigg(\frac{c_y-p_y}{\sqrt{(c_z-p_z)^2+(c_y-p_y)^2}}\Bigg) \\ 
0 & -\sin\Bigg(\frac{c_y-p_y}{\sqrt{(c_z-p_z)^2+(c_y-p_y)^2}}\Bigg) & \cos\Bigg(\frac{c_z-p_z}{\sqrt{(c_z-p_z)^2+(c_y-p_y)^2}}\Bigg)
\end{bmatrix}
\end{align}
$$
CONSTrAIN Y to U

## 3. Surfaces

### 3.1 Surface Normal at $p$

$$
\begin{align}
f(x,y,z)&=(R-\sqrt{x^2+y^2})^2+z^2-r^2=0\\
\nabla f(x,y,z)&=
\begin{bmatrix}
2(R-\sqrt{x^2+y^2})(-\frac{1}{2}(x^2+y^2)^{-0.5})(2x) \\
2(R-\sqrt{x^2+y^2})(-\frac{1}{2}(x^2+y^2)^{-0.5})(2y) \\
2z
\end{bmatrix}\\
&=\begin{bmatrix}
2x\Big[1-\frac{R}{\sqrt{x^2+y^2}}\Big]\\
2y\Big[1-\frac{R}{\sqrt{x^2+y^2}}\Big]\\
2z
\end{bmatrix}
=\text{surface normal at } (x,y,z)
\end{align}
$$

### 3.2 Tangent Plane at $p$

$$
\begin{align}
p=\begin{bmatrix}
a\\
b\\
c
\end{bmatrix}\\
\nabla f(a,b,c) \cdot\begin{bmatrix}
x\\
y\\
z\\
\end{bmatrix}
&=0\\
\end{align}
$$

$$
2a\Big[1-\frac{R}{\sqrt{a^2+b^2}}\Big]x+2b\Big[1-\frac{R}{\sqrt{a^2+b^2}}\Big]y+2cz=0
$$

### 3.3 Parametric Curve

$$
\begin{align}
f(R\cos\lambda,R\sin\lambda,r)&=\Bigg[R-\sqrt{(R\cos\lambda)^2+(R\sin\lambda)^2}\Bigg]^2+r^2-r^2\\
&=\Bigg[R-R\sqrt{\cos^2\lambda + \sin^2\lambda}\Bigg]^2+0\\
&=0\\
&\therefore \text{the curve lies on the surface}
\end{align}
$$

### 3.4 Tangent of $q$

$$
\begin{align}
\text{tangent}(q(\lambda))&=\frac{d}{d\lambda}q(\lambda)\\
&= \begin{bmatrix}
\frac{d}{d\lambda}R\cos\lambda\\
\frac{d}{d\lambda}R\sin\lambda\\
\frac{d}{d\lambda}r
\end{bmatrix}\\
&=\begin{bmatrix}
-R\sin\lambda\\
R\cos\lambda\\
0
\end{bmatrix}
\end{align}
$$

### 3.5 Tangent of $q$ on Tangent Plane

$$
\text{tangent}(q(\lambda))\in \text{tangent plane} \iff \text{tangent}(q(\lambda))\cdot\text{(tangent plane normal)}=0 \space \forall \space \lambda \in[0,2\pi]\\
$$

$$
\begin{align}

\begin{bmatrix}
-R\sin\lambda \\
R\cos\lambda \\
0
\end{bmatrix}
\cdot
\begin{bmatrix}
2x\Big[1-\frac{R}{\sqrt{x^2+y^2}}\Big]\\
2y\Big[1-\frac{R}{\sqrt{x^2+y^2}}\Big]\\
2z
\end{bmatrix}_{(x,y,z)=p}
&=
\begin{bmatrix}
-R\sin\lambda \\
R\cos\lambda \\
0
\end{bmatrix}
\cdot
\begin{bmatrix}
2R\cos\lambda\Big[1-\frac{R}{R}\Big]\\
2R\sin\lambda\Big[1-\frac{R}{R}\Big]\\
2r
\end{bmatrix}
\\
&=\begin{bmatrix}
-R\sin\lambda\\
R\cos\lambda\\
0
\end{bmatrix}
\cdot
\begin{bmatrix}
0\\
0\\
2r
\end{bmatrix}\\
&=0\\
&\therefore \text{ the tangent vector of }q \text{ lies on the tangent plane}
\end{align}
$$



## 4. Curves

### 4.1 Tangents at $P_4$

$$
\begin{align}
{B_1}(t)&=\sum_{i=0}^3{3\choose i}(1-t)^{3-i}t^iP_{i+1}\\
{B_1}'(t)&=\sum_{i=0}^3{3\choose i}P_{i+1}\Big[-(3-i)(1-t)^{2-i}t^i+(1-t)^{3-i}(i)t^{i-1}) \Big]\\
{B_1}'(1)&=\Big[{3\choose2}P_3\cdot(-1)\Big]+\Big[ {3 \choose3} P_4\cdot3\Big]\\
&=3[P_4-P_3]\\
&=\text{ray formed from } P_3\rightarrow P_4
\end{align}
$$

Replacing $P_{1,4}$ with $P_{4,7}$ will yield the same result with the substituted points and time ($t=t-1$)
$$
\begin{align}
{B_2}'(t)&=\sum_{i=0}^3{3 \choose i}P_{i+4}\Big[ -(3-i)(1-t)^{2-i}t^i +(1-t)^{3-i}(i)t^{i-1}\Big]\\
{B_2}'(0)&=3[P_5-P_4]\\
&=\text{ray formed from }P_4\rightarrow P_5
\end{align}
$$

### 4.2 Second Derivatives at $P_4$

From $4.1$,
$$
\begin{align}
{B_1}'(t)&=\sum_{i=0}^3{3\choose i}P_{i+1}\Big[-(3-i)(1-t)^{2-i}t^i+(1-t)^{3-i}(i)t^{i-1}) \Big]\\
{B_1}''(t)&=\sum_{i=0}^3{3 \choose i}P_{i+1}
	\Bigg[
	\begin{split}
		& (i-3)\big[ (i-2)(1-t)^{1-i}t^i +i(1-t)^{2-i}t^{i-1}\big] +\\
		&i\big[ (i-3)(1-t)^{2-i}t^{i-1}+(1-t)^{3-i}(i-1)t^{i-2} \big]\\
	\end{split}
	\Bigg]\\
{B_1}''(1)&=6P_2-12P_3+6P_4\\
&=6[(P_2-P_3)+(P_4-P_3)]\\
\end{align}
$$

Substituting points $P_{4,7}$
$$
{B_2}''(0)=6[(P_5-P_6)+(P_7-P_6)], \text{where t begins at 0 for }B_2
$$

### 4.3 Continuous

In order for the curve to be $C^2$ continuous given a fixed $P_1,P_2,P_3, P_4$
$$
\begin{align}
{B_1}''(1)&={B_2}''(0)\\
6[(P_2-P_3)+(P_4-P_3)]&=6[(P_5-P_6)+(P_7-P_6)]\\
P_2-P_3+P_4-P_3&=P_5-P_6+P_7-P_6\\
\Big[P_2-2P_3+P_4\Big]_\text{constant}&=P_5-2P_6+P_7
\end{align}
$$
$P_{5,7}$ are constrained by the above equality.

### 4.4 Bezier Popularity

1. Bezier curves are serializable: They can be modeled continuously by a designer/modeller, and can be serialized into a small set of discrete-valued points. A 3D program can then "deserialize" the points and recreate the exact curve that was modelled.
2. Bezier curves can be interpolated to any level of detail. Since the curve models a continuous function, the curve can be interpolated by a graphics program to any level of detail/number of linear segments.
3. Bezier curves are intuitive to design: Humans can easily model with bezier curves, since the control points alone give a rough idea of what the curve should look like. The first two and last two control points control the endpoint tangents, which is also useful in design.
4. ???