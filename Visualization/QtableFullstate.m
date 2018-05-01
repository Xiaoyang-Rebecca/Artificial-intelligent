
FULLSTATE =[1,1,5
3,3,3
5,5,4
5,1,0
5,3,2
2,5,0
];
Qtable_0 = readtable('Qread0.dat');
Qtable_1 = readtable('Qread1.dat');

%%%%%
block = cell(5,5);
PickupCells  = FULLSTATE(1:3,:);
DropoffCells = FULLSTATE(4:6,:);

%% read Q table generate input table
id=0;
sSpace0 = cell(25,1);
for i = 1:5
        for j = 1:5
            id=id+1;
            sSpace0{id} = num2str( [i,j,0]);            
        end
end
Qtable_0 = Qtable_0(:,1:4);
Qtable_0.Properties.RowNames = sSpace0;
Qtable_0.Properties.VariableNames = {'N','S','W','E'};

id=0;
sSpace1 = cell(25,1);
for i = 1:5
        for j = 1:5
            id=id+1;
            sSpace1{id} = num2str( [i,j,1]);            
        end
end

Qtable_1 = Qtable_1(:,1:4);
Qtable_1.Properties.RowNames = sSpace1;
Qtable_1.Properties.VariableNames = {'N','S','W','E'};
 

%% Preprocessing: assignment the illegal data to -1000
for i=  1:5
    for j = 1
        Loc = num2str([j,i,0]);
        Operator = 'N';
        Qtable_0{Loc,Operator} = -1000;
    end
end

for i=  1:5
    for j = 5
        Loc = num2str([j,i,0]);
        Operator = 'S';
        Qtable_0{Loc,Operator} = -1000;
    end
end

for i= 5
    for j = 1:5
        Loc = num2str([j,i,0]);
        Operator = 'E';
        Qtable_0{Loc,Operator} = -1000;
    end
end
    
for i= 1
    for j = 1:5
        Loc = num2str([j,i,0]);
        Operator = 'W';
        Qtable_0{Loc,Operator} = -1000;
    end
end
    
 %%%%%%%%%%%%%%
for i=  1:5
    for j = 1
        Loc = num2str([j,i,1]);
        Operator = 'N';
        Qtable_1{Loc,Operator} = -1000;
    end
end

for i=  1:5
    for j = 5
        Loc = num2str([j,i,1]);
        Operator = 'S';
        Qtable_1{Loc,Operator} = -1000;
    end
end

for i= 5
    for j = 1:5
        Loc = num2str([j,i,1]);
        Operator = 'E';
        Qtable_1{Loc,Operator} = -1000;
    end
end
    
for i= 1
    for j = 1:5
        Loc = num2str([j,i,1]);
        Operator = 'W';
        Qtable_1{Loc,Operator} = -1000;
    end
end
    



%% normalize:   feature scaling  to 0.2~1 for each cell
    Qtable_0_norm = Qtable_0;
    for i = 1:size(Qtable_0,1) % the length of Q table
             Legal_Agents      = Qtable_0{i,:} ;
             Legal_ID          =  (Legal_Agents~= -1000 ) ;    %  actions have -1000 value regarded as illegal actions 
             NonZero_ID         =  (Legal_Agents ~= 0 );
             NonZero_L_ID        = Legal_ID.*NonZero_ID;
             
             if sum(NonZero_L_ID) ~=0 
                 if  range(Legal_Agents(Legal_ID==1)) ==0
                     Qtable_0_norm{i,Legal_ID} = Qtable_0_norm{i,Legal_ID}./ Qtable_0_norm{i,Legal_ID};
                 else % only normalize  the legal actions to 0.2~1, illegal action keep as 0
                     Qtable_0_norm{i,Legal_ID} = ( Qtable_0{i,Legal_ID} - min(Legal_Agents(Legal_ID)) )./ range(Legal_Agents(Legal_ID==1))  *0.8 + 0.2;
                 end
             else
                 Qtable_0_norm{i,Legal_ID} = ones(size(Qtable_0_norm{i,Legal_ID}));
             end
             
             Neg_ID         = ( Legal_Agents<0);
             Zero_ID        = ( Legal_Agents==0);
             if   (sum(Zero_ID)==4)||( sum(Neg_ID)==3 && sum(Zero_ID)==1 ) || ( sum(Neg_ID)==2 && sum(Zero_ID)==2 ) ||( sum(Neg_ID)==1 && sum(Zero_ID)==3 )
                 Qtable_0_norm{i,Legal_ID} = zeros(size(Qtable_0_norm{i,Legal_ID}));
             end
             
             
    end
    Qtable_1_norm = Qtable_1;
    for i = 1:size(Qtable_1,1) % the length of Q table
             Legal_Agents      = Qtable_1{i,:} ;
             Legal_ID          =  (Legal_Agents~= -1000 ) ;    %  actions have -1000 value regarded as illegal actions 
             NonZero_ID         =  (Legal_Agents ~= 0 );
             NonZero_L_ID        = Legal_ID.*NonZero_ID;
             
             if sum(NonZero_L_ID) ~=0 
                 if  range(Legal_Agents(Legal_ID==1)) ==0
                     Qtable_1_norm{i,Legal_ID} = Qtable_1_norm{i,Legal_ID}./ Qtable_1_norm{i,Legal_ID};
                 else % only normalize  the legal actions to 0.2~1, illegal action keep as 0
                     Qtable_1_norm{i,Legal_ID} = ( Qtable_1{i,Legal_ID} - min(Legal_Agents(Legal_ID)) )./ range(Legal_Agents(Legal_ID==1))  *0.8 + 0.2;
                 end
             else
                 Qtable_1_norm{i,Legal_ID} = ones(size(Qtable_1_norm{i,Legal_ID}));
             end
             
                          
             Neg_ID         = ( Legal_Agents<0);
             Zero_ID        = ( Legal_Agents==0);
             if (sum(Zero_ID)==4) ||( sum(Neg_ID)==3 && sum(Zero_ID)==1 ) || ( sum(Neg_ID)==2 && sum(Zero_ID)==2 ) ||( sum(Neg_ID)==1 && sum(Zero_ID)==3 )
                 Qtable_1_norm{i,Legal_ID} = zeros(size(Qtable_1_norm{i,Legal_ID}));
             end
             
    end

figure()
      subplot(2,2,1)
        % plot the grids and pickup dropoff cells 
        for i = 1:5
            for j = 1:5
                  block{j,i} = [-0.5+j,0.5-i];        % location of bloc        
    %             % visualize q value as intensity of triangle
    %             N_tri =[0 1 0.5 0; 0 0 -0.5 0]  ;% triangle North
    %             S_tri =[0 1 0.5 0;-1 -1 -0.5 -1] ;% triangle Sourth
    %             W_tri =[0 0 0.5 0; 0 -1 -0.5 0]  ;% triangle North
    %             E_tri =[1 1 0.5 1;0 -1 -0.5 0] ;% triangle Sourth
    %             N_per = 1;
    %             S_per = 1;
    %             W_per = 1;
    %             E_per = 1;
    %             fill(N_tri(1,:)+ (i-1),N_tri(2,:)-(j-1),N_per*[1 0 0]); hold on 
    %             fill(S_tri(1,:)+ (i-1),S_tri(2,:)-(j-1),S_per*[0 1 0]); hold on 
    %             fill(W_tri(1,:)+ (i-1),W_tri(2,:)-(j-1),W_per*[0 0 1]); hold on 
    %             fill(E_tri(1,:)+ (i-1),E_tri(2,:)-(j-1),E_per*[1 1 0]); hold on 
                  SquareCellNodes = [0 1 1 0 0;0,0,-1,-1,0];
                  if sum(  (PickupCells(:,1) == j) .*  (PickupCells(:,2) == i)  ) ==1;  % stain pickup cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[1 1 0]); hold on 
                  end
                  if sum(  (DropoffCells(:,1) == j) .* (DropoffCells(:,2) == i) )==1;  % stain dropoff cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[0 1 0]); hold on 
                  end                  
                  
            end
        end
        % plot the q_values
        id=0;
        for i = 1:5
            for j = 1:5
                    % visualize q value as arrows
                    id=id+1;
                 
                                        % visualize q value as arrows
                  
                    if Qtable_0_norm.N(id) == -1000
                        Qtable_0_norm.N(id) =0;
                    end
                    if Qtable_0_norm.S(id)  ==-1000
                        Qtable_0_norm.S(id) =0;
                    end
                    if Qtable_0_norm.W(id)  ==-1000
                        Qtable_0_norm.W(id) =0;
                    end
                    if Qtable_0_norm.E(id)  ==-1000
                        Qtable_0_norm.W(id) =0;
                    end
                    if Qtable_0_norm.E(id)  ==-1000
                        Qtable_0_norm.E(id) =0;
                    end
                    
                    
                    N_vec = Qtable_0_norm.N(id)*[0,0.5] ;
                    S_vec = Qtable_0_norm.S(id)*[0,-0.5] ;
                    W_vec = Qtable_0_norm.W(id)*[-0.5,0] ;
                    E_vec = Qtable_0_norm.E(id)*[0.5,0] ;
                    
                    % plot all the arrows for all actions
                    quiver(block{j,i}(1),block{j,i}(2),N_vec(1),N_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),S_vec(1),S_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),W_vec(1),W_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),E_vec(1),E_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    % emphasize the active path
                    [~,AttraP_ID] = max(Qtable_0_norm{id,:});
                    switch  AttraP_ID
                        case 1 
                            quiver(block{j,i}(1),block{j,i}(2),N_vec(1),N_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 2 
                            quiver(block{j,i}(1),block{j,i}(2),S_vec(1),S_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 3 
                            quiver(block{j,i}(1),block{j,i}(2),W_vec(1),W_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 4 
                            quiver(block{j,i}(1),block{j,i}(2),E_vec(1),E_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                    end
                   
                    
%                     % qvalue of each operatiors
                     for Operator_list = {'N','S','W','E'}
                         Operator = Operator_list{1};
                         Loc = num2str([j,i,0]);
                         str = num2str( Qtable_0{Loc,Operator} );
                         str = str(1:min(4,length(str)));% only read the first 3 digits
                         if     Operator == 'N'
                             text(i-0.6,-j+0.8,str,'Color','r');hold on 
                         elseif Operator == 'S'
                             text(i-0.6,-j+0.1,str,'Color','r');hold on 
                         elseif Operator == 'W'
                             text(i-0.9  ,-j+0.5,str,'Color','r');hold on 
                         else %Operator == 'E'
                             text(i-0.2,-j+0.5,str,'Color','r');hold on                          
                         end
                     end
%                     text(-0.5+j,0.3-i,str,'Color','b');hold on                     
                 

            end
        end
        grid on ,hold on
        axis([0 5 -5 0])
       
        title('Q table for x=0 (Agent does not carry a block)')
        
     subplot(2,2,2)    
        % plot the grids and pickup dropoff cells 
         for i = 1:5
            for j = 1:5
                  block{j,i} = [-0.5+j,0.5-i];        % location of bloc        
    %             % visualize q value as intensity of triangle
    %             N_tri =[0 1 0.5 0; 0 0 -0.5 0]  ;% triangle North
    %             S_tri =[0 1 0.5 0;-1 -1 -0.5 -1] ;% triangle Sourth
    %             W_tri =[0 0 0.5 0; 0 -1 -0.5 0]  ;% triangle North
    %             E_tri =[1 1 0.5 1;0 -1 -0.5 0] ;% triangle Sourth
    %             N_per = 1;
    %             S_per = 1;
    %             W_per = 1;
    %             E_per = 1;
    %             fill(N_tri(1,:)+ (i-1),N_tri(2,:)-(j-1),N_per*[1 0 0]); hold on 
    %             fill(S_tri(1,:)+ (i-1),S_tri(2,:)-(j-1),S_per*[0 1 0]); hold on 
    %             fill(W_tri(1,:)+ (i-1),W_tri(2,:)-(j-1),W_per*[0 0 1]); hold on 
    %             fill(E_tri(1,:)+ (i-1),E_tri(2,:)-(j-1),E_per*[1 1 0]); hold on 
                  SquareCellNodes = [0 1 1 0 0;0,0,-1,-1,0];
                  if sum(  (PickupCells(:,1) == j) .*  (PickupCells(:,2) == i)  ) ==1;  % stain pickup cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[1 1 0]); hold on 
                  end
                  if sum(  (DropoffCells(:,1) == j) .* (DropoffCells(:,2) == i) )==1;  % stain dropoff cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[0 1 0]); hold on 
                  end                  
                  
            end
        end
        % plot the q_values
        id=0;
        for i = 1:5
            for j = 1:5
                    % visualize q value as arrows
                    id=id+1;
                    if Qtable_1_norm.N(id) == -1000
                        Qtable_1_norm.N(id) =0;
                    end
                    if Qtable_1_norm.S(id)  ==-1000
                        Qtable_1_norm.S(id) =0;
                    end
                    if Qtable_1_norm.W(id)  ==-1000
                        Qtable_1_norm.W(id) =0;
                    end
                    if Qtable_1_norm.E(id)  ==-1000
                        Qtable_1_norm.W(id) =0;
                    end
                    if Qtable_1_norm.E(id)  ==-1000
                        Qtable_1_norm.E(id) =0;
                    end
                                   
                    
                    N_vec = Qtable_1_norm.N(id)*[0,0.5] ;
                    S_vec = Qtable_1_norm.S(id)*[0,-0.5] ;
                    W_vec = Qtable_1_norm.W(id)*[-0.5,0] ;
                    E_vec = Qtable_1_norm.E(id)*[0.5,0] ;
                    
                    % plot all the arrows for all actions
                    quiver(block{j,i}(1),block{j,i}(2),N_vec(1),N_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),S_vec(1),S_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),W_vec(1),W_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    quiver(block{j,i}(1),block{j,i}(2),E_vec(1),E_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle',':'), hold on
                    % emphasize the active path
                    [~,AttraP_ID] = max(Qtable_1_norm{id,:});
                    switch  AttraP_ID
                        case 1 
                            quiver(block{j,i}(1),block{j,i}(2),N_vec(1),N_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 2 
                            quiver(block{j,i}(1),block{j,i}(2),S_vec(1),S_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 3 
                            quiver(block{j,i}(1),block{j,i}(2),W_vec(1),W_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                        case 4 
                            quiver(block{j,i}(1),block{j,i}(2),E_vec(1),E_vec(2),'LineWidth',2,'MaxHeadsize',1,'Color','[0 0 0]','LineStyle','-'), hold on
                    end
                   
                    
%                     % qvalue of each operatiors
                     for Operator_list = {'N','S','W','E'}
                         Operator = Operator_list{1};
                         Loc = num2str([j,i,1]);
                         str = num2str( Qtable_1{Loc,Operator} );
                         str = str(1:min(4,length(str)));% only read the first 3 digits
                         if     Operator == 'N'
                             text(i-0.6,-j+0.8,str,'Color','r');hold on 
                         elseif Operator == 'S'
                             text(i-0.6,-j+0.1,str,'Color','r');hold on 
                         elseif Operator == 'W'
                             text(i-0.9  ,-j+0.5,str,'Color','r');hold on 
                         else %Operator == 'E'
                             text(i-0.2,-j+0.5,str,'Color','r');hold on                          
                         end
                     end
%                     text(-0.5+j,0.3-i,str,'Color','b');hold on                     
                 

            end
        end
        grid on ,hold on
        axis([0 5 -5 0])

        title('Q table for x=1 (Agent carries a block)')
	  
    subplot(2,2,3)    
        % plot the grids and pickup dropoff cells
        for i = 1:5
            for j = 1:5
                  SquareCellNodes = [0 1 1 0 0;0,0,-1,-1,0];
                  if sum(  (PickupCells(:,1) == j) .*  (PickupCells(:,2) == i)  ) ==1;  % stain pickup cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[1 1 0]); hold on 
                  end
                  if sum(  (DropoffCells(:,1) == j) .* (DropoffCells(:,2) == i) )==1;  % stain dropoff cells
                    fill(SquareCellNodes(1,:)+ (i-1),SquareCellNodes(2,:)-(j-1),[0 1 0]); hold on 
                  end                  
                  
%                     % write the location of each cell on cells
                    str = strcat('(',num2str(j),',',num2str(i),')');  
                    text(-0.5+i,0.3-j,str,'Color','b');hold on 
            end
        end
        
        % plot the full state of PD world
        FullState = [PickupCells ;DropoffCells ];
        for i = 1:5
            for j = 1:5
                    % visualize q value as arrows
                    id = 1;
                    while id <= size(FullState,1)
                        if   FullState(id,1)==j  && FullState(id,2)==i
                            str = num2str(FullState(id,3));
                        else
                            str = '';
                        end 
                        
                        text(-0.5+i,0.5-j,str,'Color','r','FontSize',15);hold on 
                        id = id +1;
                    end
            end
        end
        grid on 
        axis([0 5 -5 0])   
        title('Full state of the P-D world')
        hold off
%   