%******************************************************************************%
%pathing - This function will determine the path that a robot needs to take
% in order to successfully cut a lawn.
%******************************************************************************%
function [] = pathing(yardFile)
yard = readYard(yardFile);

clf;
printYard(yard);

currentLocation = {2,20};
finished = 0;
count = 0;
justWentUp = 0;


while (~finished)
    count = count +1;
    yard{currentLocation{1}}(currentLocation{2}) = '.';
    plot(currentLocation{1}, currentLocation{2}, '.', 'Color', 'red');
    pause(0.05);
    
    if (yard{currentLocation{1}}(currentLocation{2}-1) == '0')
        currentLocation = {currentLocation{1},(currentLocation{2}-1)};
    elseif (yard{currentLocation{1}+1}(currentLocation{2}) == '0')
        currentLocation = {currentLocation{1}+1,(currentLocation{2})};
    elseif (yard{currentLocation{1}-1}(currentLocation{2}) == '0')
        currentLocation = {currentLocation{1}-1,(currentLocation{2})};
    else
        if (yard{currentLocation{1}+1}(currentLocation{2}) == '*' || yard{currentLocation{1}-1}(currentLocation{2}) == '*')
                    
            found = 0;
            while (~found)
                if (yard{currentLocation{1}}(currentLocation{2}+1) == '*')
                    currentLocation = {currentLocation{1}-1,(currentLocation{2})};
                else
                    found = 1;
                    currentLocation = {currentLocation{1},(currentLocation{2}+1)};
                end
            end
            justWentUp = 1;
            
        else
            if (yard{currentLocation{1}+1}(currentLocation{2}) == '.')
                currentLocation = {currentLocation{1}+1,(currentLocation{2})};
            elseif (yard{currentLocation{1}-1}(currentLocation{2}) == '.')
                currentLocation = {currentLocation{1}-1,(currentLocation{2})};
            end
        end
    end
    
    if (count == 10000)
        finished = 1;
    end
end

end

%******************************************************************************%
%readYard- This function will read in the sample yard configuration text.
%******************************************************************************%
function [yard] = readYard(yardFile)
yard = importdata(yardFile);
end

%******************************************************************************%
%printYard- This function will print the sample yard configuration text.
%******************************************************************************%
function [] = printYard(yard)
hold on;
for idx = 1:length(yard)
    for jdx = 1:length(yard{1})
        if(yard{idx}(jdx) == '*')
            plot(idx, jdx, '*', 'Color', 'black');
        elseif(yard{idx}(jdx) == '.')
            plot(idx, jdx, '.', 'Color', 'red');
        end
    end
end
%hold off;
end

%******************************************************************************%
%selectNewSource - This function will determine the next location that will
% be used from which to originate.
%******************************************************************************%
function [source] = selectNewSource(currentLocation, currentArea, yard)

end

%******************************************************************************%
%move - This function will determine the next move location and will execute
% that move.
%******************************************************************************%
function [] = move(currentLocation, yard, currentArea)

end