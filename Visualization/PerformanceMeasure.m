
T = readtable('Perf.xlsx','ReadRowNames',false);
T.Properties.VariableNames  = {'steps','Reward','BlocksDelivered','BankAccount','Note'};

NoteID = strcmp(T.Note,'t');

figure(1)
    subplot(2,2,1)
        plot(T.steps, T.Reward); hold on,
        scatter (T.steps(NoteID),T.Reward(NoteID),'*')  
        xlabel('Steps')
        ylabel('Reward')
        title('Performance of [Reward] for each 50 steps')
    subplot(2,2,2)
        plot(T.steps, T.BlocksDelivered);
        hold on,
        scatter (T.steps(NoteID),T.BlocksDelivered(NoteID),'*')  
        xlabel('Steps')
        ylabel('BlocksDelivered')
        title('Performance of [BlocksDelivered] for each 50 steps')        
    subplot(2,2,3)
        plot(T.steps, T.BankAccount);
        hold on,
        scatter (T.steps(NoteID),T.BankAccount(NoteID),'*')  
        xlabel('Steps')
        ylabel('BankAccount')
        title('Performance of [BankAccount] for each 50 steps')
        
        
        
