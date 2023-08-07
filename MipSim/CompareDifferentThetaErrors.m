function CompareDifferentThetaErrors(sim, thetas, dt, simTime, controller) 
    Legend = cell(length(thetas), 1);
    for ii = 1:length(thetas)
        % subplot(length(thetas), 1, ii);
        x0 = [deg2rad(thetas(ii)) 0 0 0];
        [t x u] = sim.run(x0, controller, dt, simTime);
        plot(t, rad2deg(x(1,:)))

        Legend{ii} = "$\theta_0 = "+thetas(ii)+"^\circ$";
        ylabel("$\theta$ (degrees)", "interpreter", "latex")
        xlabel("Time (seconds)")

        hold on
    end
    legend(Legend, "interpreter", "latex")
    title("Plot of system with different initial angles")
    hold off
end