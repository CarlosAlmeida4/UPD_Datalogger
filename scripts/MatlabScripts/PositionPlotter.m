clear all

data_s = yaml.loadFile("test.yaml");

%% Turn struct of struct to struct of arrays

datafields = fieldnames(data_s);
for k=1:numel(datafields)
    data_smat.(datafields{k}) = cell2mat(data_s.(datafields{k}));
end
clear data_s datafields k

%%

subplot(4,1,1)
plot(data_smat.VehiclePositionX,data_smat.VehiclePositionY)

subplot(4,1,2)
plot(data_smat.CurrentTime,data_smat.VehiclePositionX)

subplot(4,1,3)
plot(data_smat.CurrentTime,data_smat.VehiclePositionY)

subplot(4,1,4)
vehSpeedKph = data_smat.VehicleSpeed * 3.6;
plot(data_smat.CurrentTime,vehSpeedKph)
clear vehSpeedKph