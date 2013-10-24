package com.pixelballoon.pixelboost;

import android.app.Activity;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.content.Context;

import java.io.File;
import java.io.FileDescriptor;
import java.io.IOException;
import java.util.Locale;

public class PixelboostHelpers
{
	public static PixelboostActivity mainActivity;

	public static void quit()
	{
		mainActivity.moveTaskToBack(true);
	}

	public static String getCurrentLocale()
	{
		return Locale.getDefault().getLanguage();
	}

	public static String getBundleFilePath()
	{
		Context context = mainActivity.getApplication();
		return context.getApplicationInfo().sourceDir;
	}

	public static String getSaveFilePath()
	{
		Context context = mainActivity.getApplication();
		String filesDir = context.getFilesDir().getAbsolutePath();
		File directory = new File(filesDir + "/");
		directory.mkdirs();
		return filesDir;
	}

	public static void openUrl(String url)
	{
		Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		mainActivity.startActivity(browserIntent);
	}

	public static void playMusic(String filename, float volume)
	{
		Context context = mainActivity.getApplication();
    	AssetManager assetManager = context.getAssets();

    	try 
    	{
    		AssetFileDescriptor fileDescriptor = assetManager.openFd(filename);
 
			if (fileDescriptor != null)
			{
				if (mainActivity.music != null)
				{
					stopMusic();
				}

				mainActivity.music = new MediaPlayer();
				mainActivity.music.setDataSource(fileDescriptor.getFileDescriptor(), fileDescriptor.getStartOffset(), fileDescriptor.getLength());
				fileDescriptor.close();
				mainActivity.music.setLooping(true);
				mainActivity.music.prepare();
				mainActivity.music.start();

				setMusicVolume(volume);
			}
		} catch (IOException e) {}
	}

	public static void stopMusic()
	{
		if (mainActivity.music != null)
		{
			mainActivity.music.stop();
			mainActivity.music.release();
			mainActivity.music = null;
		}
	}

	public static void setMusicVolume(float volume)
	{
		if (mainActivity.music != null)
			mainActivity.music.setVolume(volume, volume);
	}

	public static int loadSound(String filename)
	{
		Context context = mainActivity.getApplication();
    	AssetManager assetManager = context.getAssets();

    	try 
    	{
    		AssetFileDescriptor fileDescriptor = assetManager.openFd(filename);
 
			if (fileDescriptor != null)
			{
				int soundId = mainActivity.sounds.load(fileDescriptor.getFileDescriptor(), fileDescriptor.getStartOffset(), fileDescriptor.getLength(), 1);
				fileDescriptor.close();
				return soundId;
			}
    	} catch (IOException e) {}

    	return 0;
	}

	public static int playSound(int soundId, float volume, float pitch)
	{
		return mainActivity.sounds.play(soundId, volume, volume, 0, 0, pitch);
	}

	public static void stopSound(int soundId)
	{
		mainActivity.sounds.stop(soundId);
	}

	public static void setSoundVolume(int soundId, float volume)
	{
		mainActivity.sounds.setVolume(soundId, volume, volume);
	}

	public static void setSoundLooping(int soundId, boolean looping)
	{
		mainActivity.sounds.setLoop(soundId, looping ? -1 : 0);
	}

	public static void setSoundPitch(int soundId, float pitch)
	{
		mainActivity.sounds.setRate(soundId, pitch);
	}
}
