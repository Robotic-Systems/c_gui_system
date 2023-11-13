from PIL import Image, ImageDraw, ImageFont

# Configuration
output_width = 102  # Width of the output image
output_height = 64  # Height of the output image
font_size = 19  # Font size
text = "Hello World!\nHow Are You?"  # Text to convert to a bitmap
font_path = r"C:\Users\Pat\Documents\1. Robotic systems\3. Goals\2023 Q3 Goals\dev\c_gui_system\FontOrignals\jupiter-crash-brk.regular.ttf"  # Path to your font file
output_file = "output_bitmap.png"  # Output image file
alignment = "left"
# Create a blank image with white background
image = Image.new("RGB", (output_width, output_height), "white")
draw = ImageDraw.Draw(image)

# Load the font
font = ImageFont.truetype(font_path, font_size)

# Calculate the size of the text to be placed

text_size = draw.textsize(text, font=font)

# Calculate the position to center the text

input_x = 0
input_y = 0
if alignment == 'left':
    x = 0  # Adjust as needed
    y = 0  # Adjust as needed
elif alignment == 'right':
    # Calculate the coordinates for top right based on the given position
    x = input_x - text_size[0]  # input_x is the x-coordinate for the top right corner
    y = input_y    
elif alignment == 'center':
    x = (output_width - text_size[0]) / 2
    y = (output_height - text_size[1]) / 2

# Draw the text on the image
draw.text((x, y), text, fill="black", font=font)

# Save the image as a bitmap
# image.save(output_file)
image.show()

# Convert the image to greyscale
image = image.convert('L')
# Invert the grayscale image
inverted_image = Image.eval(image, lambda x: 255 - x)

# Save the inverted greyscale pixel values to a text file
with open('output.txt', 'w') as text_file:
    for row in range(output_height):
        text_file.write('{')
        for col in range(output_width):
            pixel_value = inverted_image.getpixel((col, row))
            text_file.write(f'{pixel_value:3d}, ')
        text_file.write('},\n')

print(f"Bitmap saved as {output_file}")

