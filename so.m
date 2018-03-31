clear
%Open text file that contains names of wav files
% fp = fopen([dirin,namelist],'r');
fp = fopen(['Serial\','names.txt'], 'r');


Filename = fscanf(fp,'%s', 1);

no_files = 0;

%Count number of wav files in the class
while strcmp(Filename,'') ~= 1
    no_files = no_files +1;
    Filename = fscanf(fp,'%s\n', 1);  %9
    
end
fclose(fp);
% fGI = fopen([dirout,namelist],'wt'); % GI namelist
% fp = fopen([dirin,namelist],'r');
fp = fopen(['Serial\','names.txt'], 'r');

testf = 1;
%Start reading wav files
for i = 1:no_files
    Fnamein = fscanf(fp,'%s\n', 1);
    if(testf)
        %read only file name, without the class
        fscanf(fp,'%s',1);
    end
%     pitch_fname = [Fnamein(1:end-3),'mat'];%fscanf(fpch,'%s\n', 1);
    %read audio (.wav) file
    %disp([dirin,Fnamein]);
%     [samples,Fs] = audioread([dirin,Fnamein]);
    
end