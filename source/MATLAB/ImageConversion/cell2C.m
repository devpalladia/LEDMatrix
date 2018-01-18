function [c_string] = cell2C(name, cell)
    s = size(cell);
    c_string = 'const int ';
    c_string = horzcat(c_string, name, '[', int2str(s(1)), ']', '[', int2str(s(2)), '] = {');
    
    for i=1:s(1)
        c_string = sprintf('%s\n{', c_string);
        for j=1:s(2)
            c_string = horzcat(c_string, cell{i,j});
            if (j ~= s(2))
                c_string = horzcat(c_string,', ');
            end
        end
        if (i ~= s(1))
            c_string = horzcat(c_string, '},');
        else
            c_string = horzcat(c_string, '}');
        end
    end
    c_string = sprintf('%s\n};', c_string);
end

