from PIL import Image
from loguru import logger as log
def resize_image(input_image_path, output_text_path, width, height):
    # Open the input image
    image = Image.open(input_image_path)
    
    # Resize the image to the specified width and height
    image = image.resize((width, height))
    
    # Convert the image to greyscale
    image = image.convert('1')
    
    # Create the output text file
    with open(output_text_path, 'w') as text_file:
        pass  # Empty file creation
        
    # Save the greyscale pixel values to a text file
    with open(output_text_path, 'w') as text_file:
        for row in range(height):
            for col in range(width):
                pixel_value = image.getpixel((col, row))
                # if pixel_value > 0:
                #     pixel_value =0
                # else:
                #     pixel_value =1
                text_file.write(f'{pixel_value:03}, ')
            text_file.write('\n')

if __name__ == "__main__":
    input_image_path = r'output_bitmap.png'  # Change this to your input image file
    output_text_path = 'output.txt'  # Output text file
    target_width = 102  # Change to the desired width
    target_height = 64  # Change to the desired height

    resize_image(input_image_path, output_text_path, target_width, target_height)
    log.info("OUTPUT TEXT FILE")
