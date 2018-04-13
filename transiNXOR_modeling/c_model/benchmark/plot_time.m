
model_sizes = int64(2.^(1:8));
N = 100000;

out_file = 'runtime.csv';

if exist(out_file, 'file')==2
  delete(out_file);
end

for i = 1:length(model_sizes)
    model_size = model_sizes(i);
    system('make clean');
    make_command = sprintf('make CFLAGS=''-DMODEL_SIZE=%i''', model_size);
    [status, ~] = system(make_command, '-echo');
    if (status ~= 0)
        break;
    end
    run_command = sprintf('./benchmark_size %i %i %s', N, model_size, out_file);
    [status, ~] = system(run_command, '-echo');
    if (status ~= 0)
        fprintf('Error running benchmark for N = %d\n', model_size);
        break;
    end
    %[status, ~] = system(make_command);
end

result = csvread(out_file);
loglog(result(:, 1), result(:, 2), '-*');
xticks(result(:, 1));
yticks(sort(result(:, 2)));
xlabel('Model Size');
ylabel('Run time (s)');
grid on