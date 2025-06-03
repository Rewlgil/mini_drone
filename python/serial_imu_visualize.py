"""
    File: serial_imu_visualize.py
    Brief: Visualize IMU data using cube rotation.
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
import serial
import rotate_cube

def main():
    """
    Main loop recieve IMU data from serial and rotate cube.

    """
    COM_PORT = 'COM6'
    BAUDRATE = 9600
    
    WINDOW_SIZE =  200
    ROTATE_SPEED = 1
    SCALE = 50

    # Configure the serial port
    ser = serial.Serial(port=COM_PORT,
                        baudrate=BAUDRATE,
                        timeout=1)

    # Initialize all imported Pygame modules
    pygame.init()

    # Set up the display
    window = pygame.display.set_mode((WINDOW_SIZE, WINDOW_SIZE))
    pygame.display.set_caption("Cube")
    clock = pygame.time.Clock()

    angle_x = 0
    angle_y = 0
    angle_z = 0

    game_running_flag = True

    while game_running_flag:
        clock.tick(10)

        try:
            # Open the serial port if not already open
            if not ser.is_open:
                ser.open()

            # Read a line of data (ends with a newline character)
            data_line = ser.readline().decode('utf-8').strip()

            # If data is OK
            if data_line != 0:
                print(data_line)
                try:
                    [angle_x, angle_y] = [int(ang) for ang in data_line.split('\t')]
                except:
                    pass

        except Exception as e:
            print(f"Error: {e}")
            game_running_flag = False

        # Limit the angle value
        angle_x = rotate_cube.limit_ang(angle_x)
        angle_y = rotate_cube.limit_ang(angle_y)
        angle_z = rotate_cube.limit_ang(angle_z)

        # Clear window display
        window.fill((0, 0, 0))

        rotate_cube.draw_cube(window, SCALE, WINDOW_SIZE / 2, angle_x, angle_y, angle_z)
        rotate_cube.draw_axis(window, SCALE, WINDOW_SIZE / 2, angle_x, angle_y, angle_z)

        pygame.display.update()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_running_flag = False

    pygame.quit()

    # Close the serial port
    if ser.is_open:
        ser.close()

if __name__ == '__main__':
    main()
