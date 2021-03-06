clear all
close all
matlab_info;

likelihood4 = flipud(likelihood4);
likelihood7 = flipud(likelihood7);
likelihood8 = flipud(likelihood8);
likelihood9 = flipud(likelihood9);
% 4,7    9,4     9,7     9,8
classes = [4,7; 9,4; 9,7; 9,8];
like = {0 0 0 likelihood4 0 0 likelihood7 likelihood8 likelihood9};

        figure;
    surf(likelihood4,'EdgeColor','None');
    view(2); 
    title('likelihood of class4')
            figure;
    surf(likelihood7,'EdgeColor','None');
    view(2); 
    title('likelihood of class7')
            figure;
    surf(likelihood8,'EdgeColor','None');
    view(2); 
    title('likelihood of class8')
            figure;
    surf(likelihood9,'EdgeColor','None');
    view(2); 
    title('likelihood of class9')

for r = 1:4
    odds_ratio1 = log10(like{classes(r,1)}./like{classes(r,2)});
    figure;
    surf(odds_ratio1,'EdgeColor','None');
    view(2); 
    title(strcat(strcat('odds ', num2str(classes(r,1))), strcat('/', num2str(classes(r,2)))))
    
    odds_ratio2 = log10(like{classes(r,2)}./like{classes(r,1)});
    figure;
    surf(odds_ratio2,'EdgeColor','None');
    view(2); 
    title(strcat(strcat('odds ', num2str(classes(r,2))), strcat('/', num2str(classes(r,1))))) 
end

%    odds_ratio1 = 


% 2D map using view
%figure;
%surf(Z,'EdgeColor','None');
%view(2); 