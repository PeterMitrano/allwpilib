package edu.wpi.first.wpilib.plugins.simulation;

import java.io.File;
import java.util.Properties;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.Status;
import org.eclipse.ui.IStartup;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

import edu.wpi.first.wpilib.plugins.core.WPILibCore;
import edu.wpi.first.wpilib.plugins.core.ant.AntPropertiesParser;
import edu.wpi.first.wpilib.plugins.simulation.installer.SimulationInstaller;

public class WPILibSimulationPlugin extends AbstractUIPlugin implements IStartup{

	// The plug-in ID
	public static final String PLUGIN_ID = "WPILib_Java_Robot_Development"; //$NON-NLS-1$

	// The shared instance
	private static WPILibSimulationPlugin plugin;
	
	private static BundleContext context;

	static BundleContext getContext() {
		return context;
	}

	/*
	 * (non-Javadoc)
	 * @see org.osgi.framework.BundleActivator#start(org.osgi.framework.BundleContext)
	 */
	public void start(BundleContext bundleContext) throws Exception {
		WPILibSimulationPlugin.context = bundleContext;
	}

	/*
	 * (non-Javadoc)
	 * @see org.osgi.framework.BundleActivator#stop(org.osgi.framework.BundleContext)
	 */
	public void stop(BundleContext bundleContext) throws Exception {
		WPILibSimulationPlugin.context = null;
	}

	/**
	 * Returns the shared instance
	 *
	 * @return the shared instance
	 */
	public static WPILibSimulationPlugin getDefault() {
		return plugin;
	}

	public String getCurrentVersion() {
		try {
			Properties props = new AntPropertiesParser(WPILibSimulationPlugin.class.getResourceAsStream("/resources/configuration.properties")).getProperties();
			if (props.getProperty("version").startsWith("$")) {
				return "DEVELOPMENT";
			} else {
				return props.getProperty("version");
			}
		} catch (CoreException e) {
			WPILibSimulationPlugin.logError("Error getting properties.", e);
			return "DEVELOPMENT";
		}
	}
	
	public static void logInfo(String msg) {
		getDefault().getLog().log(new Status(Status.INFO, PLUGIN_ID, Status.OK, msg, null));
	}
	
	private static void logError(String msg, CoreException e) {
		getDefault().getLog().log(new Status(Status.ERROR, PLUGIN_ID, Status.OK, msg, e));
	}

	public String getJavaPath() {
		return WPILibCore.getDefault().getWPILibBaseDir()
				+ File.separator + "java" + File.separator + "current";
	}
	
	@Override
	public void earlyStartup() {
		new SimulationInstaller(getCurrentVersion()).installIfNecessary();
		
	}
	
}
