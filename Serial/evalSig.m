function [] = evalSig(currentSiggy)

    plot(currentSiggy/10, 'color', 'black', 'LineWidth', 2);

    digiIdx = [];
    hold on

    for idx = 1:length(currentSiggy) - 1
        if (abs(currentSiggy(idx)-currentSiggy(idx+1)) > 1.5)
            digiIdx(size(digiIdx,1) + 1, :) = idx;
            plot(digiIdx, 0, '*');
        end
    end

    a = (1/9600)*1000000;

%     for jdx = 1:length(digiIdx)-1
%         if (abs(digiIdx(jdx+1) - digiIdx(jdx)) < a)
%             a = abs(digiIdx(jdx+1) - digiIdx(jdx));
%         end
%     end

    masterDigi = [];
    for idx = 1:length(digiIdx)-1

        if (floor((digiIdx(idx+1)-digiIdx(idx))/a) > 1)

            temp = digiIdx(idx):a:digiIdx(idx+1);
            masterDigi(size(masterDigi,1) + 1, :) = digiIdx(idx);
            for jdx = 2:length(temp) - 1
                masterDigi(size(masterDigi,1) + 1, :) = temp(jdx);
            end
        else
            masterDigi(size(masterDigi,1) + 1, :) = digiIdx(idx);
        end
    end

    masterDigi(size(masterDigi,1) + 1, :) = digiIdx(length(digiIdx));
    masterDigi(size(masterDigi,1) + 1, :) = masterDigi(length(masterDigi))+a;
    

    for idx = 1:length(masterDigi)
        line([masterDigi(idx),masterDigi(idx)], [-.1,0.5], 'LineStyle', ':', 'color', 'black', 'LineWidth', 2)
    end
    data = [];
    for idx = 1:length(masterDigi)-1
        currX = (masterDigi(idx+1)-masterDigi(idx))/2 + masterDigi(idx) - 20;
        if currentSiggy(floor(currX)) > .5
            data(size(data,1) + 1, :) = 1;
            text(currX, -.03, '1', 'FontSize', 18);
        else
            data(size(data,1) + 1, :) = 0;
            text(currX, -.03, '0', 'FontSize', 18);
        end
    end

    ylim([-1,1]);
    xlim([2000,6000]);
    axis off
    
    dd = 1:10:length(masterDigi)-1;
    for idx = 1:10:length(masterDigi)-1
        drawbrace([masterDigi(idx+8),-0.1], [masterDigi(idx),-0.1],10, 'LineWidth', 2, 'Color', 'black'); 
        drawbrace([masterDigi(idx+10),-0.1], [masterDigi(idx+8),-0.1],10, 'LineWidth', 2, 'Color', 'black');         
    end

end