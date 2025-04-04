import numpy as np
import matplotlib.pyplot as plt

def generate_mountain_map(width, height, peak_height):
	grid = np.zeros((height, width), dtype=int)
	peak_x, peak_y = width // 2, height // 3

	for y in range(height):
		for x in range(width):
			distance = abs(peak_x - x) + abs(peak_y - y)
			value = max(0, peak_height - distance * (peak_height // height))
			grid[y, x] = value

	return grid

def display_map(grid):
	for row in grid:
		print(" ".join(f"{val:2}" for val in row))

width, height, peak_height = 19, 30, 100
mountain_map = generate_mountain_map(width, height, peak_height)
display_map(mountain_map)
