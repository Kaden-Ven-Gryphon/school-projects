% script ditherGIF - dithers the fifty images in homework1_animation_frames and
% sews them into a gif
% 

%apply dither to each of 50 images
for i = 1:50
    fileName = sprintf("homework1_animation_frames/frame_%d_delay-0.1s.gif", i);
    
    %read image and convert from byte indexed to rgb
    %Frames to read, specified as the comma-separated pair consisting of 'Frames' 
    %and a positive integer, a vector of integers, or 'all'. 
    %For example, if you specify the value 3, imread reads the third frame in the file. 
    %If you specify 'all', then imread reads all frames and returns them in the order in which they appear in the file.
    
    [indexedSrcImg, colorMap] = imread(fileName, "Frames", 1);
    sourceImg = ind2rgb(indexedSrcImg, colorMap);

    
    
    %apply dither to image--in HW1 you must implement this function and include orderedDither.m. 
    ditheredImg = orderedDither(sourceImg);
    
    %stitch image into the gif
    %format image into byte indexed image
    [indexedImg, colorMap] = rgb2ind(ditheredImg,256);
    
    %write indexed image to its own file
    writeName = sprintf("homework1_dithered_frames/frame_%d_delay-0.1s.gif", i);
    imwrite(indexedImg, colorMap, writeName, 'gif');
    
    %write indexed image to the gif
    if i == 1
      imwrite(indexedImg, colorMap, 'homework1_dithered_frames/dithered_animation.gif', 'gif', 'DelayTime',0.1, 'Loopcount', inf);
    else
      imwrite(indexedImg, colorMap, 'homework1_dithered_frames/dithered_animation.gif', 'gif', 'DelayTime',0.1, 'WriteMode', 'append');
    end
end
