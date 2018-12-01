import numpy as np

num_samples = 100000000
R = 1
xs = np.random.uniform(-1.0, 1.0, num_samples)
ys = np.random.uniform(-1.0, 1.0, num_samples)

distances2center = xs**2 + ys**2
points_fall_in_circle = distances2center[distances2center <= R**2]

print 'Number of points (N) sampled within the rectangle ([-1,-1], [1,1]): {}'.format(num_samples)
print 'Number of points (M) fall into the circle inscribed inside the rectangle: {}'.format(points_fall_in_circle.shape[0])
print 'Estimated PI = 4 * M/N = {}'.format(4 * float(points_fall_in_circle.shape[0]) / num_samples)
