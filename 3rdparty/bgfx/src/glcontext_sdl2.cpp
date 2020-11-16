/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "bgfx_p.h"

#if (BGFX_CONFIG_RENDERER_OPENGLES || BGFX_CONFIG_RENDERER_OPENGL)
#	include "renderer_gl.h"

#	if BGFX_USE_SDL2

namespace bgfx { namespace gl
{

#	define GL_IMPORT(_optional, _proto, _func, _import) _proto _func = NULL
#	include "glimports.h"

	struct SwapChainGL
	{
		SwapChainGL(SDL_Window* _window, SDL_GLContext _context)
			: m_window(_window)
		{
			SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
			m_context = SDL_GL_CreateContext(m_window);
			SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
			BX_CHECK(NULL != m_context, "Create swap chain failed: %s", SDL_GetError() );

			makeCurrent();
			GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f) );
			GL_CHECK(glClear(GL_COLOR_BUFFER_BIT) );
			swapBuffers();
			GL_CHECK(glClear(GL_COLOR_BUFFER_BIT) );
			swapBuffers();
            		SDL_GL_MakeCurrent(m_window, _context);
		}

		~SwapChainGL()
		{
            		//EGLSurface defaultSurface = eglGetCurrentSurface(EGL_DRAW);
            		SDL_GLContext defaultContext = SDL_GL_GetCurrentContext();
			SDL_GL_MakeCurrent(m_window, 0);
			SDL_GL_DeleteContext(m_context);
            		SDL_GL_MakeCurrent(m_window, defaultContext);
		}

		void makeCurrent()
		{
			SDL_GL_MakeCurrent(m_window, m_context);
		}

		void swapBuffers()
		{
			SDL_GL_SwapWindow(m_window);
		}

		SDL_GLContext m_context;
		SDL_Window* m_window;
	};

	void GlContext::create(uint32_t _width, uint32_t _height)
	{
		BX_UNUSED(_width, _height);
		m_window = (SDL_Window*)g_platformData.nwh;
		BGFX_FATAL(m_window != NULL, Fatal::UnableToInitialize, "Failed to retrieve SDL2/GLES window");
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
		m_context = SDL_GL_CreateContext(m_window);
		BGFX_FATAL(m_context != NULL, Fatal::UnableToInitialize, "Failed to create context.");
		int success = SDL_GL_MakeCurrent(m_window, m_context);
		if (success != 0)
			BX_TRACE("cannot set sdl/gl context %s", SDL_GetError());
		m_current = NULL;
		SDL_GL_SetSwapInterval(0);
		import();
		g_internalData.context = m_context;
	}

	void GlContext::destroy()
	{
		if (NULL != m_window)
		{
			SDL_GL_MakeCurrent(m_window, 0);
			SDL_GL_DeleteContext(m_context);
			m_context = NULL;
		}
	}

	void GlContext::resize(uint32_t _width, uint32_t _height, uint32_t _flags)
	{
#	if BX_PLATFORM_EMSCRIPTEN
		EMSCRIPTEN_CHECK(emscripten_set_canvas_element_size(HTML5_TARGET_CANVAS_SELECTOR, _width, _height) );
#	else
		if (NULL != m_window)
		{
			SDL_SetWindowSize(m_window, _width, _height);
		}
		BX_UNUSED(_width, _height);
#	endif // BX_PLATFORM_*

		if (NULL != m_window)
		{
			bool vsync = !!(_flags&BGFX_RESET_VSYNC);
			SDL_GL_SetSwapInterval(vsync ? 1 : 0);
		}
	}

	uint64_t GlContext::getCaps() const
	{
		return BX_ENABLED(0
						| BX_PLATFORM_LINUX
						| BX_PLATFORM_WINDOWS
						| BX_PLATFORM_ANDROID
						)
			? BGFX_CAPS_SWAP_CHAIN
			: 0
			;
	}

	SwapChainGL* GlContext::createSwapChain(void* _nwh)
	{
		return BX_NEW(g_allocator, SwapChainGL)(m_window, m_context);
	}

	void GlContext::destroySwapChain(SwapChainGL* _swapChain)
	{
		BX_DELETE(g_allocator, _swapChain);
	}

	void GlContext::swap(SwapChainGL* _swapChain)
	{
		makeCurrent(_swapChain);

		if (NULL == _swapChain)
		{
			if (NULL != m_window)
			{
				SDL_GL_SwapWindow(m_window);
			}
		}
		else
		{
			_swapChain->swapBuffers();
		}
	}

	void GlContext::makeCurrent(SwapChainGL* _swapChain)
	{
		if (m_current != _swapChain)
		{
			m_current = _swapChain;

			if (NULL == _swapChain)
			{
				if (NULL != m_window)
				{
					SDL_GL_MakeCurrent(m_window, m_context);
				}
			}
			else
			{
				_swapChain->makeCurrent();
			}
		}
	}

	void GlContext::import()
	{
		BX_TRACE("Import:");
#		define GL_EXTENSION(_optional, _proto, _func, _import)                           \
			{                                                                            \
				if (NULL == _func)                                                       \
				{                                                                        \
					_func = reinterpret_cast<_proto>(SDL_GL_GetProcAddress(#_import) );      \
					BX_TRACE("\t%p " #_func " (" #_import ")", _func);                   \
					BGFX_FATAL(_optional || NULL != _func                                \
						, Fatal::UnableToInitialize                                      \
						, "Failed to create OpenGLES context. SDL2_GL_GetProcAddress(\"%s\")" \
						, #_import);                                                     \
				}                                                                        \
			}


#	include "glimports.h"

#	undef GL_EXTENSION
	}

} /* namespace gl */ } // namespace bgfx

#	endif // BGFX_USE_SDL2
#endif // (BGFX_CONFIG_RENDERER_OPENGLES || BGFX_CONFIG_RENDERER_OPENGL)
