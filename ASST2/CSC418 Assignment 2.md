# CSC418 Assignment 2

**Michael Vu**

**<CDF ID>**

## 1. Transformations

### 1.1 2D Affine Transformation

$$
TA=B\\
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
=
\begin{bmatrix}
b_{1x} & b_{2x} & b_{3x}\\
b_{1y} & b_{2y} & b_{3y}\\
c_1 & c_2 & c_3
\end{bmatrix}
\\
$$



## 2. Viewing and Projection

## 3. Surfaces

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