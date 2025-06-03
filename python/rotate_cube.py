"""
    File: rotate_cube.py
    Brief: Display an rotated cube represent the orientation.
    Author: Rewlgil
    Date: 03 June 2025

    Copyright (c) 2025 Rewlgil
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
"""

import pygame
import numpy as np
from math import *

def limit_ang(ang_deg):
    """
    Limit angle between -360 to 360 degree

    Parameters
    ----------
    ang_deg : int
        Input angle
    
    Returns
    ----------
    ang_limit_deg : int
        Output limited angle
    """
    sign = -1 if ang_deg < 0 else 1
    ang_limit_deg = sign * (abs(ang_deg) % 360)

    return ang_limit_deg

def rotate_point(src_pt, ang_x, ang_y, ang_z):
    """
    Rotate 3D point around x, y, z axis with specified angle.

    Parameters
    ----------
    src_pt : array_like
        Array shape 3x1 represent position in 3D plane
    ang_x : int
        Rotation angle in degree around x axis
    ang_y : int
        Rotation angle in degree around y axis
    ang_z : int
        Rotation angle in degree around z axis

    Returns
    ----------
    dst_pt : array_like
        Array shape 3x1 represent position in 3D plane
    """
    # Convert angle from degree unit to radian
    ang_x_rad = radians(ang_x)
    ang_y_rad = radians(ang_y)
    ang_z_rad = radians(ang_z)

    # Rotation matrix
    rot_x_mat = np.array([[1, 0, 0],
                          [0, cos(ang_x_rad), -sin(ang_x_rad)],
                          [0, sin(ang_x_rad), cos(ang_x_rad)]])

    rot_y_mat = np.array([[cos(ang_y_rad), 0, sin(ang_y_rad)],
                          [0, 1, 0],
                          [-sin(ang_y_rad), 0, cos(ang_y_rad)]])

    rot_z_mat = np.array([[cos(ang_z_rad), -sin(ang_z_rad), 0],
                          [sin(ang_z_rad), cos(ang_z_rad), 0],
                          [0, 0, 1]])

    dst_pt = np.dot(rot_z_mat, np.dot(rot_y_mat, np.dot(rot_x_mat, src_pt)))
    
    return dst_pt

def projection_point(src_3d_pt):
    """
    Project 3D point on 2D plane.

    Parameters
    ----------
    src_3d_pt : array_like
        Array shape 3x1 represent position in 3D plane

    Returns
    ----------
    dst_2d_pt : array_like
        Array shape 2x1 represent position in 2D plane according to src_3d_pt
    """

    proj_matrix = np.array([[1, 0, 0],
                            [0, 1, 0],
                            [0, 0, 0]])

    dst_pt = np.dot(proj_matrix, src_3d_pt)
    dst_2d_pt = dst_pt[0:2]

    return dst_2d_pt

def scale_2d_point(src_2d_pt, scale, offset):
    """
    Scale and offset point in 2D plane.
    
    Parameters
    ----------
    src_2d_pt : array_like
        Array shape 2x1 represent position in 2D plane
    scale : int
        Scale factor
    offset : int
        Offset value

    Returns
    ----------
    dst_2d_pt : array_like
        Array shape 2x1 represent position in 2D plane
    """
    dst_2d_pt = (src_2d_pt * scale) + offset
    
    return dst_2d_pt

def connect_points(window, i, j, points, color):
    """
    Draw connected line in given window.
    
    Parameters
    ----------
    window : pygame surface object 
        Surface to draw line
    i, j : int
        Index of point in array points for use as line end point
    points : array_like
        Array of 2D point (array 2x1)
    color : tuple
        Tuple represent line RGB color
    """
    pygame.draw.line(window, color, 
                     (points[i][0][0], points[i][1][0]), 
                     (points[j][0][0], points[j][1][0]))

def draw_cube(window, scale=1, offset=0, ang_x=0, ang_y=0, ang_z=0):
    """
    Draw white cube on 2D plane

    Parameters
    ----------
    window : pygame surface object 
        Surface to draw line
    scale : int
        Scale factor
    offset : int
        Offset value
    ang_x : int
        Angle to rotate in degree around x axis
    ang_y : int
        Angle to rotate in degree around y axis
    ang_z : int
        Angle to rotate in degree around z axis
    """

    # Cube vertex point in 3D plane
    cube_vertexs_3d = list()
    cube_vertexs_3d.append(np.array([[-1],[-1],[ 1]]))
    cube_vertexs_3d.append(np.array([[ 1],[-1],[ 1]]))
    cube_vertexs_3d.append(np.array([[ 1],[ 1],[ 1]]))
    cube_vertexs_3d.append(np.array([[-1],[ 1],[ 1]]))
    cube_vertexs_3d.append(np.array([[-1],[-1],[-1]]))
    cube_vertexs_3d.append(np.array([[ 1],[-1],[-1]]))
    cube_vertexs_3d.append(np.array([[ 1],[ 1],[-1]]))
    cube_vertexs_3d.append(np.array([[-1],[ 1],[-1]]))

    # Rotate, projection and scale cube in 2D plane
    cube_vertexs_2d = list()
    for point in cube_vertexs_3d:
        rot_3d_pt = rotate_point(point, ang_x, ang_y, ang_z)
        rot_2d_pt = projection_point(rot_3d_pt)
        rot_scale_2d_pt = scale_2d_point(rot_2d_pt, scale, offset)
        cube_vertexs_2d.append(rot_scale_2d_pt)

    # Draw cube edge
    connect_points(window, 0, 1, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 0, 3, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 0, 4, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 1, 2, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 1, 5, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 2, 3, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 2, 6, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 3, 7, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 4, 5, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 4, 7, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 5, 6, cube_vertexs_2d, (255, 255, 255))
    connect_points(window, 6, 7, cube_vertexs_2d, (255, 255, 255))

def draw_axis(window, scale=1, offset=0, ang_x=0, ang_y=0, ang_z=0):
    """
    Draw 3D axis.

    Parameters
    ----------
    window : pygame surface object 
        Surface to draw line
    scale : int
        Scale factor
    offset : int
        Offset value
    ang_x : int
        Angle to rotate in degree around x axis
    ang_y : int
        Angle to rotate in degree around y axis
    ang_z : int
        Angle to rotate in degree around z axis
    """
    # Point to draw axis mark
    ax_line_end_pt_3d = list()
    ax_line_end_pt_3d.append(np.array([[ 0],[ 0],[ 0]]))
    ax_line_end_pt_3d.append(np.array([[ 2],[ 0],[ 0]]))
    ax_line_end_pt_3d.append(np.array([[ 0],[ 2],[ 0]]))
    ax_line_end_pt_3d.append(np.array([[ 0],[ 0],[ 2]]))

    # Rotate, projection and scale axis mark in 2D plane
    ax_line_end_pt_2d = list()
    for point in ax_line_end_pt_3d:
        rot_3d_pt = rotate_point(point, ang_x, ang_y, ang_z)
        rot_2d_pt = projection_point(rot_3d_pt)
        rot_scale_2d_pt = scale_2d_point(rot_2d_pt, scale, offset)
        ax_line_end_pt_2d.append(rot_scale_2d_pt)

    # Draw axis line
    connect_points(window, 0, 1, ax_line_end_pt_2d, (255, 0, 0))
    connect_points(window, 0, 2, ax_line_end_pt_2d, (0, 255, 0))
    connect_points(window, 0, 3, ax_line_end_pt_2d, (0, 0, 255))

def main():
    """
    Main loop display cube and axis. Rotate with user keyboard input.

    """
    WINDOW_SIZE =  200
    ROTATE_SPEED = 1
    SCALE = 50

    # Initialize all imported Pygame modules
    pygame.init()

    # Set up the display
    window = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
    pygame.display.set_caption("Cube")
    clock = pygame.time.Clock()

    game_running_flag = True

    angle_x = 0
    angle_y = 0
    angle_z = 0

    while game_running_flag:
        clock.tick(60)
        window.fill((0, 0, 0))

        draw_cube(window, SCALE, WINDOW_SIZE / 2, angle_x, angle_y, angle_z)
        draw_axis(window, SCALE, WINDOW_SIZE / 2, angle_x, angle_y, angle_z)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_running_flag = False

            keys = pygame.key.get_pressed()
            if keys[pygame.K_r]:
                angle_y = 0
                angle_x = 0
                angle_z = 0
            elif keys[pygame.K_a]:
                angle_y += ROTATE_SPEED
            elif keys[pygame.K_d]:
                angle_y -= ROTATE_SPEED      
            elif keys[pygame.K_w]:
                angle_x += ROTATE_SPEED
            elif keys[pygame.K_s]:
                angle_x -= ROTATE_SPEED
            elif keys[pygame.K_q]:
                angle_z -= ROTATE_SPEED
            elif keys[pygame.K_e]:
                angle_z += ROTATE_SPEED

            angle_x = limit_ang(angle_x)
            angle_y = limit_ang(angle_y)
            angle_z = limit_ang(angle_z)

        pygame.display.update()

    pygame.quit()

if __name__ == '__main__':
    main()
