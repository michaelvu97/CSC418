# CSC418 Assignment 2

**Michael Vu**

**<CDF ID>**

## 1. Transformations

## 2. Viewing and Projection

## 3. Surfaces

## 4. Curves

### 4.1 Tangents at $P_4$

$$
\begin{align}
{B_1}(t)&=\sum_{i=0}^3{3\choose i}(1-t)^{3-i}t^iP_i\\
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
{B_1}''(1)&=6P_2-12P_3+6P_4
\end{align}
$$
