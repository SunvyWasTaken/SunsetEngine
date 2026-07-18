#pragma once

namespace Sunset
{
    class EditorApplication;

    /**
     * Gives the selected application an opportunity to configure the editor.
     *
     * A game target selected through SUNSET_EDITOR_APPLICATION_TARGET must
     * provide this function. The editor supplies a no-op implementation when
     * no application target is selected.
     */
    void ConfigureEditorApplication(EditorApplication& app);
}
