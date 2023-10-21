classdef (Abstract) AnimatedSim < Sim
    properties
        axisVals = [-1 10 -1 10];
        animationDt = 1/30;
    end
    methods (Abstract)
        updateObjs(this, x)
    end
    methods
        function [frames x inputs] = animateWithComputedU(o, x0, controller, dt, Tf)
        % ANIMATEWITHCOMPUTEDU Animates simulation starting at x0 from 0 to Tf
        %   and returns frames to be saved into animation using MipSim.saveAnimation()
            arguments
                o MipSim
                x0
                controller Controller
                dt
                Tf
            end
        
            [t x inputs] = o.run(x0, controller, dt, Tf);

            o.updateObjs(x(:,1));

            frames = [getframe(gcf)];
            prevT = 0;
            tic;
            for ii=2:length(t)
                if t(ii) - prevT < o.animationDt
                    continue;
                end
                prevT = t(ii);

                clf;
                title("Time = "+t(ii)+" Theta = "+rad2deg(x(1,ii))+" Phi = "+rad2deg(x(2,ii))+" u = "+inputs(ii));
                
                o.updateObjs(x(:,ii));
                drawnow;
                frames = [frames getframe(gcf)];
                pause(o.animationDt-toc);
                tic;
            end

        end

        function saveAnimation(o, filename, frames)
        % SAVEANIMATION Saves frames from animation into avi file.

            video = VideoWriter('Animation.avi');
            video.Quality = 95;
            video.FrameRate = 1/o.animationDt;
            open(video);
            for frame=frames
                writeVideo(video,frame);
            end
            close(video)
        end

        function xyRange(obj, xVal, bottom)
        % XYRANGE   Sets xy range and bottom y value for animation
        %   obj.XYRANGE([-1 10], -1) sets min x value in range to -1 and 
        %   max x value in range to 10. Also sets bottom of window to -1.
            obj.axisVals = [xVal(1) xVal(2) bottom bottom+xVal(2)-xVal(1)];
        end

    end
end