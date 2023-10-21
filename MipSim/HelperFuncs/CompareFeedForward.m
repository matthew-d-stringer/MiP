function CompareFeedForward(sim, x0, dt, simTime, controller, feedForwardFunc)
    subplot(2,1,2)
    controller.removeFeedForwardFunc();
    [t x u] = sim.run(x0, controller, dt, simTime);
    plot(t, rad2deg(x(1,:)))
    title("Without Feedforward")
    ylabel("Theta (degrees)")
    xlabel("Time (seconds)")
    % ylim([-270 270])

    subplot(2,1,1)
    controller.reset();
    controller.addFeedForwardFunc(feedForwardFunc);
    [t x u] = sim.run(x0, controller, dt, simTime);
    plot(t, rad2deg(x(1,:)))
    title("With Feedforward")
    ylabel("Theta (degrees)")
    xlabel("Time (seconds)")
    % ylim([-270 270])
end