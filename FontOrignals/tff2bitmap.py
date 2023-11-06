from PIL import Image, ImageDraw, ImageFont

# Configuration
output_width = 102  # Width of the output image
output_height = 64  # Height of the output image
font_size = 19  # Font size
text_num = "0123456789"  # Text to convert to a bitmap
text_lc = "abcdefghijklmnopqrstuvwxyz"
text_uc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
text_sym = " `~!@#$%^&*()-_=+[]\{\}|;':\",./<>?"
font_path = r"C:\Users\Pat\Documents\1. Robotic systems\3. Goals\2023 Q3 Goals\dev\c_gui_system\FontOrignals\jupiter-crash-brk.regular.ttf"  # Path to your font file

output_file = "output_bitmap.png"  # Output image file

# Create a blank image with white background
image = Image.new("RGB", (output_width, output_height), "white")
draw = ImageDraw.Draw(image)

# Load the font
font = ImageFont.truetype(font_path, font_size)

# Calculate the position to center the text
total_text_width = 0  # Total width of all characters
text = text_lc + text_uc + text_sym + text_num

for char in text:
    char_bbox = draw.textbbox((0, 0), char, font)
    char_width = char_bbox[2] - char_bbox[0]
    total_text_width += char_width

x = (output_width - total_text_width) / 2
y = (output_height - font_size) / 2  # Assuming font size is the maximum character height

# Draw each character on the image and print its width
for char in text:
    char_bbox = draw.textbbox((0, 0), char, font)
    char_width = char_bbox[2] - char_bbox[0]
    draw.text((x, y), char, fill="black", font=font)
    print(f"Character '{char}' width: {char_width} pixels")
    x += char_width

# Save the image as a bitmap
image.save(output_file)

print(f"Bitmap saved as {output_file}")
