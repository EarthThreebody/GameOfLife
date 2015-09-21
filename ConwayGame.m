%https://zh.wikipedia.org/wiki/%E5%BA%B7%E5%A8%81%E7%94%9F%E5%91%BD%E6%B8%B8%E6%88%8F
function ConwayGame
 
m = 25;
n = 25;
h = 1000;%繁殖代数
a = round(rand(m,n));%产生m*n矩阵随机初始值，0,1各50%
 
[xx,yy] = find(a==1);
for i=1:length(xx)
    rectangle('Position',[xx(i)-1,yy(i)-1,1,1],'FaceColor','g');
end

for k=1:h
    delete(gca);
    rectangle('Position',[0,0,m,n],'FaceColor','k');
    b = a(:,[end,1:end-1])+a(:,[2:end,1])+a([end,1:end-1],:)+a([end,1:end-1],[end,1:end-1])+...
        a([end,1:end-1],[2:end,1])+a([2:end,1],:)+a([2:end,1],[end,1:end-1])+a([2:end,1],[2:end,1]);
    c = zeros(m,n);
 
    [b2x,b2y] = find(b==2);
    for i=1:length(b2x)
        c(b2x(i),b2y(i)) = a(b2x(i),b2y(i));%周围有两个存活->保持状态
    end

    [b3x,b3y] = find(b==3);
    for i=1:length(b3x)
        c(b3x(i),b3y(i)) = 1;%周围有三个存活->繁殖
    end
 
    [xx,yy] = find(c==1);
    for i=1:length(xx)
        rectangle('Position',[xx(i)-1,yy(i)-1,1,1],'FaceColor','g');
    end
    axis square;
    title(['Generation: ',num2str(k)]);
    pause(.1)
    a = c;
end
