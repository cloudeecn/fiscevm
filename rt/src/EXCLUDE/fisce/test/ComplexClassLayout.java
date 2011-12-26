package EXCLUDE.fisce.test;

import java.util.HashMap;
import java.util.LinkedHashMap;

import fisce.util.SimpleList;

public class ComplexClassLayout {

	public static void main(String[] args) {
		Scene s = new Stage0();
		s.load();
	}
}

abstract class Scene {
	private int currentFrame;

	private long nextWakeupTime;

	private long currentTime;

	public final long nspf;

	private long fpsTime;

	private long processTime;

	private long renderTime;

	// protected final Layer rootNode = new Layer("root");

	protected final LinkedHashMap<String, Object> layers = new LinkedHashMap<String, Object>();

	protected final HashMap<String, Object> sprites = new HashMap<String, Object>();

	protected final HashMap<String, Object> textboxes = new HashMap<String, Object>();

	protected final SimpleList<String> toRemove = new SimpleList<String>();

	protected boolean throwSIE;

	private boolean running = true;

	public Scene(long nspf) {
		this.nspf = nspf;
	}

	protected abstract void load();
	
	
}

abstract class PhysicalScene extends Scene {
	public final Object world;
	public final float ratioPTM;

	public PhysicalScene(long nspf, float pixelToMeterRatio) {
		super(nspf);
		ratioPTM = pixelToMeterRatio;
		world = new Object();
	}

}

class Stage extends PhysicalScene {
	protected boolean black;
	protected Object player;
	protected float playerX;
	protected float playerY;
	protected String backB;
	protected String backW;
	public boolean controlOn = true;
	protected Object dead;
	protected Object switchSE;

	public Stage() {
		super(1000000000 / 30, 51 / 1.7f);// 40000000);
	}

	protected Object whiteLayer;
	protected Object blackLayer;
	protected Object playerLayer;
	protected Object backgroundLayer;
	protected Object textboxLayer;
	protected Object background;
	protected Object textbox;

	public void load() {
		if (layers == null) {
			TestService.fail("failed...");
		}
	}
}

class Stage0 extends Stage {

}