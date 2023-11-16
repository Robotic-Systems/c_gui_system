from PIL import Image, ImageDraw, ImageFont

# Configuration
output_width = 102  # Width of the output image
output_height = 64  # Height of the output image
font_size = 19  # Font size
text = "one: 1"  # Text to convert to a bitmap
font_path = r"C:\Users\Pat\Documents\1. Robotic systems\3. Goals\2023 Q3 Goals\dev\c_gui_system\FontOrignals\jupiter-crash-brk.regular.ttf"  # Path to your font file
output_file = "output_bitmap.png"  # Output image file
horizontal_alignment = "left"
vertical_alignment = "top"  # Vertical alignment option
input_x = 0
input_y = 0
invert_image = False  # Boolean flag for image inversion

# Create a blank image with white background
image = Image.new("RGB", (output_width, output_height), "white")
draw = ImageDraw.Draw(image)

# Load the font
font = ImageFont.truetype(font_path, font_size)

# Calculate the size of the text to be placed
text_size = draw.textsize(text, font=font)

# Calculate the position to center the text
# Horizontal alignment
if horizontal_alignment == 'left':
    x = input_x
elif horizontal_alignment == 'right':
    x = input_x - text_size[0]
elif horizontal_alignment == 'center':
    x = input_x - text_size[0] / 2

# Vertical alignment
if vertical_alignment == 'top':
    y = input_y
elif vertical_alignment == 'bottom':
    y = input_y - font_size
elif vertical_alignment == 'center':
    y = input_y - text_size[1] / 2

# Draw the text on the image
draw.text((x, y), text, fill="black", font=font)



# Convert the image to greyscale
image = image.convert('L')

# Check if image inversion is needed
if not invert_image:
    image.save(output_file)
    image.show()
    image = Image.eval(image, lambda x: 255 - x)
else:
    image = image.copy()
    imageinv = Image.eval(image, lambda x: 255 - x)
    imageinv.save(output_file)
    imageinv.show()



# Save the inverted greyscale pixel values to a text file
with open('output.txt', 'w') as text_file:
    for row in range(output_height):
        text_file.write('{')
        for col in range(output_width):
            pixel_value = image.getpixel((col, row))
            text_file.write(f'{pixel_value:3d}, ')
        text_file.write('},\n')

print(f"Bitmap saved as {output_file}")
