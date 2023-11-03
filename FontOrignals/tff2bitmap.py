from PIL import Image, ImageDraw, ImageFont

# Configuration
output_width = 102  # Width of the output image
output_height = 64  # Height of the output image
font_size = 19  # Font size
text_num = "0123456789"  # Text to convert to a bitmap
text_lc  = "abcdefghijklmnopqrstuvwxyz"
text_uc  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
text_sym = " `~!@#$%^&*()-_=+[]\{\}|;':\",./<>?"
font_path = r"C:\Users\Pat\Documents\1. Robotic systems\3. Goals\2023 Q3 Goals\dev\c_gui_system\FontOrignals\jupiter-crash-brk.regular.ttf"  # Path to your font file

# font_path = r"C:\Users\Pat\Documents\1. Robotic systems\3. Goals\2023 Q3 Goals\dev\c_gui_system\FontOrignals\calibri.regular.ttf"  # Path to your font file
output_file = "output_bitmap.png"  # Output image file

# Create a blank image with white background
image = Image.new("RGB", (output_width, output_height), "white")
draw = ImageDraw.Draw(image)

# Load the font
font = ImageFont.truetype(font_path, font_size)
# text = text_lc + "\n" + text_uc + "\n" + text_sym +"\n" + text_num
text = "Hello World"
# Calculate the position to center the text
text_bbox = draw.textbbox((0, 0), text, font)
text_width = text_bbox[2] - text_bbox[0]
text_height = text_bbox[3] - text_bbox[1]

x = (output_width - text_width) / 2
y = (output_height - text_height) / 2

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

