function DitherImg = orderedDither(inputImg)
%ORDEREDDITHER Takes rgb image and returns dithered black/white image
%   This returns a black/white image color is lost.  This should work on
%   images that are 0-255 or 0-1

%this is the 4x4 BayerMatrix,  it is already divied by 16 and has 0.5
%subtracted
BayerMatrix = [-0.5, 0, -0.375, 0.125; 0.25, -0.25, 0.375, -0.125; -0.3125, 0.1875, -0.4375, 0.0625; 0.4375, -0.0625, 0.3125, -0.1875];
%the size of the matrix used to find the indexes
n = 4;

%convert the input to a gray 2d image
grayImg = rgb2gray(inputImg);

%calculate the range of colors.  This automaticly correct for images that
%are 0-1 (doubles) or 0-255(uint8)
r = max(grayImg) - min(grayImg);

%z is used to figure out the threshold in this case setting it to the range
%is close enought to work
z = r;

newImg = grayImg;
[rows, cols]=size(grayImg);

%loop through each pixel
for i=1:rows
    for j=1:cols

        %find the corisponding index for the dithermatrix
        bayerI = mod(i,n) +1;
        bayerJ = mod(j,n) +1;

        %value from the ditherMatrix
        bayer_value = BayerMatrix(bayerI, bayerJ);

        %multiply the bayerVal by the range to convert it to a scale
        %simular to the image, and add the value to the original
        output_color = grayImg(i,j) + (r*bayer_value);
        
        %if the value is above the threshold it is white other wise it is
        %black
        if output_color > (z/ 2)
            newImg(i,j) = 255;
        else
            newImg(i,j) = 0;
        end
      
    end
end

%convert the 2d image into 3d image and output image from function
DitherImg = cat(3, newImg, newImg, newImg);
end

