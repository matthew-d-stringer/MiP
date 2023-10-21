function C = LoadControllerFromMatFile(filename)
%LOADCONTROLLERFROMMATFILE Loads the controller tf from a SISOTOOL session.
% Syntax: C = LoadControllerFromMatFile(filename)

    load(filename);
    Controller = ControlSystemDesignerSession.DesignerData.Designs(end);
    C = Controller.Data.C;
    fprintf("Choosing Design: %s\n", Controller.Name)
    
end