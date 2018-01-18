function [converted_image] = convertImage(image, target_rows, target_columns)
    I = imread(image);
    II = imresize(I, [target_rows, target_columns]);
    imwrite(II, sprintf('resized_%s', image));
    converted_image = cell(target_rows, target_columns);
    for i=1:target_rows
        for j=1:target_columns
            rgb(1) = II(i,j,1);
            rgb(2) = II(i,j,2);
            rgb(3) = II(i,j,3);
            converted_image{i,j} = rgb2hex(rgb);
        end
    end
end

