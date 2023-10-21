classdef (Abstract) AnimatedSim < Sim
    methods (Abstract)
        updateObjs(this, x)
    end
    methods
        function frames = animateWithComputedU(o, x0, controller, dt, Tf)
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
    end
end