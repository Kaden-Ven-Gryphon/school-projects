%load the images and cast to double so they can multiply by the convertion
%matrix
eldenRingImg = double(imread('eldenring.png'));
mandrilImg = double(imread('mandril_color.png'));

%the output image
chromaERimg = eldenRingImg;
chromaMandrilImg = mandrilImg;

%the covnertion matixes
rgbToChroma = [0.299, 0.587, 0.114; -0.16874, -0.33126, 0.5; 0.5, -0.41869, -0.08131];
chromaToRgb = [1, 0, 1.402; 1, -0.34414, -0.71414; 1, 1.772, 0];



%convert eldenring to chroma
[rows, colms, ~] = size(eldenRingImg);
for i=1:rows
    for j=1:colms
        %get pixel from original
        pixel = [0;0;0];
        pixel(1, 1) = eldenRingImg(i, j, 1);
        pixel(2, 1) = eldenRingImg(i, j, 2);
        pixel(3, 1) = eldenRingImg(i, j, 3);
        outPixel = [0;0;0];
        %matrix multiply the conversion matix to the pixel
        pixel = rgbToChroma*pixel;
        %add to Cb Cr
        pixel(2, 1) = pixel(2, 1) + 128;
        pixel(3, 1) = pixel(3, 1) + 128;
        %set the pixel of the output
        chromaERimg(i, j, 1) = pixel(1, 1);
        chromaERimg(i, j, 2) = pixel(2, 1);
        chromaERimg(i, j, 3) = pixel(3, 1);
    end
end

%convert madril to chroma
[rows, colms, ~] = size(mandrilImg);
for i=1:rows
    for j=1:colms
        pixel = [0;0;0];
        pixel(1, 1) = mandrilImg(i, j, 1);
        pixel(2, 1) = mandrilImg(i, j, 2);
        pixel(3, 1) = mandrilImg(i, j, 3);
        pixel = rgbToChroma*pixel;
        pixel(2, 1) = pixel(2, 1) + 128;
        pixel(3, 1) = pixel(3, 1) + 128;
        chromaMandrilImg(i, j, 1) = pixel(1, 1);
        chromaMandrilImg(i, j, 2) = pixel(2, 1);
        chromaMandrilImg(i, j, 3) = pixel(3, 1);

    end
end

%preform the subsampling for eldenring
subsamp1ERimg = chromaERimg;
subsamp2ERimg = chromaERimg;
[rows, colms, ~] = size(chromaERimg);
for i=1:2:rows-1
    for j=1:2:colms-1
        %sample Cb
        %right 1
        subsamp1ERimg(i,j+1,2) = chromaERimg(i,j,2);
        %down 1
        subsamp1ERimg(i+1,j,2) = chromaERimg(i,j,2);
        %right 1 down 1
        subsamp1ERimg(i+1,j+1,2) = chromaERimg(i,j,2);
        %sample Cr
        %right 1
        subsamp1ERimg(i,j+1,3) = chromaERimg(i,j,3);
        %down 1
        subsamp1ERimg(i+1,j,3) = chromaERimg(i,j,3);
        %right 1 down 1
        subsamp1ERimg(i+1,j+1,3) = chromaERimg(i,j,3);
        
        %sample method 2
        %sampling Cb is the same as above
        subsamp2ERimg(i,j+1,2) = chromaERimg(i,j,2);
        subsamp2ERimg(i+1,j,2) = chromaERimg(i,j,2);
        subsamp2ERimg(i+1,j+1,2) = chromaERimg(i,j,2);

        %sample Cr
        %up 1
        subsamp2ERimg(i,j,3) = chromaERimg(i+1,j,3);
        %right 1 up 1
        subsamp2ERimg(i,j+1,3) = chromaERimg(i+1,j,3);
        %right 1
        subsamp2ERimg(i+1,j+1,3) = chromaERimg(i+1,j,3);
        
    end
end

%preform sub sampling on the mandril img
subsamp1Mandril = chromaMandrilImg;
subsamp2Mandril = chromaMandrilImg;
[rows, colms, ~] = size(chromaMandrilImg);
for i=1:2:rows
    for j=1:2:colms
        subsamp1Mandril(i,j+1,2) = chromaMandrilImg(i,j,2);
        subsamp1Mandril(i+1,j,2) = chromaMandrilImg(i,j,2);
        subsamp1Mandril(i+1,j+1,2) = chromaMandrilImg(i,j,2);
        subsamp1Mandril(i,j+1,3) = chromaMandrilImg(i,j,3);
        subsamp1Mandril(i+1,j,3) = chromaMandrilImg(i,j,3);
        subsamp1Mandril(i+1,j+1,3) = chromaMandrilImg(i,j,3);

        subsamp2Mandril(i,j+1,2) = chromaMandrilImg(i,j,2);
        subsamp2Mandril(i+1,j,2) = chromaMandrilImg(i,j,2);
        subsamp2Mandril(i+1,j+1,2) = chromaMandrilImg(i,j,2);
        subsamp2Mandril(i,j,3) = chromaMandrilImg(i+1,j,3);
        subsamp2Mandril(i,j+1,3) = chromaMandrilImg(i+1,j,3);
        subsamp2Mandril(i+1,j+1,3) = chromaMandrilImg(i+1,j,3);
        
    end
end






ERoutputImg1 = eldenRingImg;
ERoutputImg2 = eldenRingImg;

%convert eldenring to rgb for subsample 1
[rows, colms, ~] = size(eldenRingImg);
for i=1:rows
    for j=1:colms
        pixel = [0;0;0];
        pixel(1, 1) = subsamp1ERimg(i, j, 1);
        pixel(2, 1) = subsamp1ERimg(i, j, 2);
        pixel(3, 1) = subsamp1ERimg(i, j, 3);
        pixel(2,1) = pixel(2,1) - 128;
        pixel(3,1) = pixel(3,1) - 128;
        pixel = chromaToRgb*pixel;
        ERoutputImg1(i, j, 1) = pixel(1, 1);
        ERoutputImg1(i, j, 2) = pixel(2, 1);
        ERoutputImg1(i, j, 3) = pixel(3, 1);
    end
end

%convert eldenring to rgb for subsample 2
[rows, colms, ~] = size(eldenRingImg);
for i=1:rows
    for j=1:colms
        pixel = [0;0;0];
        pixel(1, 1) = subsamp2ERimg(i, j, 1);
        pixel(2, 1) = subsamp2ERimg(i, j, 2);
        pixel(3, 1) = subsamp2ERimg(i, j, 3);
        pixel(2,1) = pixel(2,1) - 128;
        pixel(3,1) = pixel(3,1) - 128;
        pixel = chromaToRgb*pixel;
        ERoutputImg2(i, j, 1) = pixel(1, 1);
        ERoutputImg2(i, j, 2) = pixel(2, 1);
        ERoutputImg2(i, j, 3) = pixel(3, 1);
    end
end

mandrilOut1 = subsamp1Mandril;
mandrilOut2 = subsamp1Mandril;

%convert mandril to rgb subsample 1
[rows, colms, ~] = size(subsamp1Mandril);
for i=1:rows
    for j=1:colms
        pixel = [0;0;0];
        pixel(1, 1) = subsamp1Mandril(i, j, 1);
        pixel(2, 1) = subsamp1Mandril(i, j, 2);
        pixel(3, 1) = subsamp1Mandril(i, j, 3);
        pixel(2,1) = pixel(2,1) - 128;
        pixel(3,1) = pixel(3,1) - 128;
        pixel = chromaToRgb*pixel;
        mandrilOut1(i, j, 1) = pixel(1, 1);
        mandrilOut1(i, j, 2) = pixel(2, 1);
        mandrilOut1(i, j, 3) = pixel(3, 1);
    end
end

%convert mandril to rgb subsample 2
[rows, colms, ~] = size(subsamp2Mandril);
for i=1:rows
    for j=1:colms
        pixel = [0;0;0];
        pixel(1, 1) = subsamp2Mandril(i, j, 1);
        pixel(2, 1) = subsamp2Mandril(i, j, 2);
        pixel(3, 1) = subsamp2Mandril(i, j, 3);
        pixel(2,1) = pixel(2,1) - 128;
        pixel(3,1) = pixel(3,1) - 128;
        pixel = chromaToRgb*pixel;
        mandrilOut2(i, j, 1) = pixel(1, 1);
        mandrilOut2(i, j, 2) = pixel(2, 1);
        mandrilOut2(i, j, 3) = pixel(3, 1);
    end
end
%same the images as pngs
eldenRingImg = uint8(eldenRingImg);
mandrilImg = uint8(mandrilImg);
ERoutputImg1= uint8(ERoutputImg1);
imwrite(ERoutputImg1, 'eldenringSubSample1.png');
ERoutputImg2= uint8(ERoutputImg2);
imwrite(ERoutputImg2, 'eldenringSubSample2.png');
mandrilOut1 = uint8(mandrilOut1);
imwrite(mandrilOut1, 'mandrilSubSample1.png');
mandrilOut2 = uint8(mandrilOut2);
imwrite(mandrilOut2, 'mandrilSubSample2.png');

%get a 4x2 sample of the the three eldenring pictures
sampleER0 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleER0(i,j,k) = eldenRingImg(300+i,300+j,k);
        end
    end
end
sampleER0 = uint8(sampleER0);
imwrite(sampleER0, 'sample444ER.png');

sampleER1 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleER1(i,j,k) = ERoutputImg1(300+i,300+j,k);
        end
    end
end
sampleER1 = uint8(sampleER1);
imwrite(sampleER1, 'sample4201ER.png');

sampleER2 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleER2(i,j,k) = ERoutputImg2(300+i,300+j,k);
        end
    end
end
sampleER2 = uint8(sampleER2);
imwrite(sampleER2, 'sample4202ER.png');

%get 4x2 sample of the three mandril pictures
sampleMan0 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleMan0(i,j,k) = mandrilImg(200+i,200+j,k);
        end
    end
end
sampleMan0 = uint8(sampleMan0);
imwrite(sampleMan0, 'sample444Man.png');

sampleMan1 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleMan1(i,j,k) = mandrilOut1(200+i,200+j,k);
        end
    end
end
sampleMan1 = uint8(sampleMan1);
imwrite(sampleMan1, 'sample4201Man.png');

sampleMan2 = zeros(2,4,3);
for i=1:2
    for j=1:4
        for k=1:3
            sampleMan2(i,j,k) = mandrilOut2(200+i,200+j,k);
        end
    end
end
sampleMan2 = uint8(sampleMan2);
imwrite(sampleMan2, 'sample4202Man.png');
